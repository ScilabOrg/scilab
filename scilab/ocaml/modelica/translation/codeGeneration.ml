(*
 *  Translator from Modelica 2.x to flat Modelica
 *
 *  Copyright (C) 2005 – 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

open ErrorDico (* To have access to GenericError *)

type flat_instance =
  {
    variables: Instantiation.component_description list;
    equations: Instantiation.equation_desc list
  }

and function_description =
  {
    inputs: (string * Types.class_specifier) list;
    outputs: (string * Types.class_specifier) list
  }

(* Utilities *)

let evaluate t = Lazy.force t


(* Code generation functions *)

let rec generate_code filename fun_dir inst_defs =
  let fun_defs =
    List.fold_left collect_function_definitions [] inst_defs in
  let dir_name = create_function_directory fun_dir in
  Unix.chdir dir_name;
  for i = 1 to List.length fun_defs do
    generate_function_definition i (List.nth fun_defs (i - 1))
  done;
  Unix.chdir "..";
  List.iter (generate_instance_element filename fun_defs) inst_defs

and create_function_directory fun_dir = match fun_dir with
  | None ->
      begin try Unix.chdir "functions"; Unix.chdir ".." with
        | Unix.Unix_error ((Unix.EACCES | Unix.ENOENT), _, _) ->
            Unix.mkdir "functions" 0o777
      end;
      "functions"
  | Some filename -> filename

and collect_function_definitions fun_defs (id, elt_desc) =
  match evaluate elt_desc.Instantiation.element_nature with
    | Instantiation.Class cl_def ->
        let ctx =
          {
            path = cl_def.Instantiation.class_path;
            location = cl_def.Instantiation.class_location;
            instance_nature = Instantiation.ClassElement
          } in
        let cl_spec = cl_def.Instantiation.class_type in
        fun_defs @ collect_function_definitions_in_class_specifier ctx cl_spec
    | Instantiation.Component cpnt_desc ->
        let ctx =
          {
            path = cpnt_desc.Instantiation.component_path;
            location = cpnt_desc.Instantiation.component_location;
            instance_nature =
              Instantiation.ComponentElement cpnt_desc.Instantiation.class_name
          } in
        fun_defs @ collect_function_definitions_in_component ctx cpnt_desc

and collect_function_definitions_in_class_specifier ctx = function
  | Types.PredefinedType _ | Types.ComponentType _ | Types.ArrayType _ |
    Types.TupleType _ -> []
  | Types.ClassType cl_type ->
      collect_function_definition_in_class_type ctx cl_type

and collect_function_definition_in_class_type ctx cl_type =
  let fun_defs = collect_inner_functions ctx cl_type in
  collect_function ctx fun_defs cl_type

and collect_inner_functions ctx cl_type =
  match evaluate cl_type.Types.kind with
    | Types.Class | Types.Model | Types.Block | Types.Package |
      Types.Function ->
        let named_elts = cl_type.Types.named_elements in
        List.fold_left (collect_function_definitions_in_type ctx) [] named_elts
    | Types.Record | Types.ExpandableConnector | Types.Connector -> []

and collect_function_definitions_in_type ctx fun_defs (id, elt_type) =
  let elt_type' = evaluate elt_type in
  match elt_type'.Types.dynamic_scope, elt_type'.Types.element_nature with
    | None, Types.ClassElement cl_spec ->
        let ctx' = {ctx with
                      path = ctx.path @ [Instantiation.Name id]} in
        let cl_spec' = evaluate cl_spec in
        let fun_defs' =
          collect_function_definitions_in_class_specifier ctx' cl_spec' in
        fun_defs' @ fun_defs
    | Some _, _ |
      None,
      (Types.ComponentElement _ | Types.ComponentTypeElement _ |
       Types.PredefinedTypeElement _) -> fun_defs

and collect_function ctx fun_defs cl_type =
  match evaluate cl_type.Types.kind with
    | Types.Function ->
        let named_elts = cl_type.Types.named_elements in
        function_description_of_named_elements ctx named_elts :: fun_defs
    | Types.Class | Types.Model | Types.Block | Types.Record |
      Types.ExpandableConnector | Types.Connector| Types.Package -> fun_defs

and function_description_of_named_elements ctx named_elts =
  let collect_input_or_output (id, elt_type) fun_desc =
    let collect_input_or_output' cpnt_type =
      match evaluate cpnt_type.Types.causality with
        | Types.Input ->
            let cl_spec = evaluate cpnt_type.Types.base_class in
            { fun_desc with inputs = (id, cl_spec) :: fun_desc.inputs }
        | Types.Output ->
            let cl_spec = evaluate cpnt_type.Types.base_class in
            { fun_desc with outputs = (id, cl_spec) :: fun_desc.outputs }
        | Types.Acausal -> fun_desc in
    let elt_type' = evaluate elt_type in
    match elt_type'.Types.element_nature with
      | Types.ComponentElement cpnt_type ->
          collect_input_or_output' cpnt_type
      | Types.ClassElement _ | Types.ComponentTypeElement _ |
        Types.PredefinedTypeElement _ -> fun_desc in
  let fun_desc = { inputs = []; outputs = [] } in
  ctx,
  List.fold_right collect_input_or_output named_elts fun_desc

and collect_function_definitions_in_component ctx cpnt_desc =
  match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.DynamicArray cpnt_desc ->
        let ctx' =
          {ctx with path = ctx.path @ [Instantiation.Index 0]} in
        collect_function_definitions_in_component ctx' cpnt_desc
    | Instantiation.Instance inst ->
        let elts = evaluate inst.Instantiation.elements in
        let named_elts = elts.Instantiation.named_elements in
        List.fold_left collect_function_definitions [] named_elts
    | Instantiation.PredefinedTypeInstance _ -> []
    | Instantiation.StaticArray [||] -> []
    | Instantiation.StaticArray cpnt_descs ->
        let ctx' =
          {ctx with path = ctx.path @ [Instantiation.Index 0]} in
        collect_function_definitions_in_component ctx' cpnt_descs.(0)

and generate_function_definition i (ctx, fun_desc) =
  let filename = Printf.sprintf "Function%d" i
  and name = string_of_path ctx.path in
  generate_modelica_function_definition ctx filename name fun_desc;
  generate_c_function_prototype ctx filename name fun_desc

and generate_modelica_function_definition ctx filename name fun_desc =
  let oc = open_out (Printf.sprintf "%s.mo" filename) in
  try
    Printf.fprintf oc "function %s \"%s\"\n" filename name;
    List.iter (generate_modelica_function_input ctx oc) fun_desc.inputs;
    List.iter (generate_modelica_function_output ctx oc) fun_desc.outputs;
    Printf.fprintf oc "external;\nend %s;\n" filename;
    close_out oc
  with exn -> close_out oc; raise exn

and generate_modelica_function_input ctx oc = function
  | id, Types.PredefinedType { Types.base_type = Types.RealType } ->
      Printf.fprintf oc "\t%s Real %s;\n" "input" id
  | id, _ ->
      (*invalid_arg "generate_modelica_function_element"*)
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_NonRealFuncInput"; id];
          err_info = [];
          err_ctx = ctx })

and generate_modelica_function_output ctx oc = function
  | id, Types.PredefinedType { Types.base_type = Types.RealType } ->
      Printf.fprintf oc "\t%s Real %s;\n" "output" id
  | id, _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_NonRealFuncOutput"; id];
          err_info = [];
          err_ctx = ctx })

and generate_c_function_prototype ctx filename name fun_desc =
  let oc = open_out (Printf.sprintf "%s.h" filename) in
  try
    Printf.fprintf oc "/* C prototype of %s */\n" name;
    begin match fun_desc.outputs with
      | [] -> Printf.fprintf oc "void %s(" filename
      | [_, Types.PredefinedType { Types.base_type = Types.RealType }] ->
          Printf.fprintf oc "double %s(" filename
      | _ ->
          (*invalid_arg "generate_c_function_prototype"*)
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_FuncWithManyOutputs"];
              err_info = [];
              err_ctx = ctx })
    end;
  generate_c_function_input_arguments ctx oc fun_desc.inputs;
  Printf.fprintf oc ");\n";
  close_out oc
  with exn -> close_out oc; raise exn

and generate_c_function_input_arguments ctx oc = function
  | [id, Types.PredefinedType { Types.base_type = Types.RealType }] ->
      Printf.fprintf oc "\n\tdouble %s" id
  | (id, Types.PredefinedType { Types.base_type = Types.RealType }) :: inputs ->
      Printf.fprintf oc "\n\tdouble %s," id;
      generate_c_function_input_arguments ctx oc inputs
  | _ -> invalid_arg "generate_c_function_input_arguments"

and generate_instance_element filename fun_defs (id, elt_desc) =
  match evaluate elt_desc.Instantiation.element_nature with
    | Instantiation.Class _ -> ()
    | Instantiation.Component cpnt_desc ->
        generate_component_description filename fun_defs id cpnt_desc

and generate_component_description filename fun_defs id cpnt_desc =
  let flat_inst = collect_component_elements cpnt_desc in
  let filename = match filename with
    | None -> id ^ ".mo"
    | Some s -> s in
  let oc = open_out filename in
  try
    let ctx =
      {path = cpnt_desc.Instantiation.component_path;
       location = cpnt_desc.Instantiation.component_location;
       instance_nature =
         Instantiation.ComponentElement cpnt_desc.Instantiation.class_name} in
    generate_flatten_instance ctx fun_defs oc id flat_inst;
    close_out oc
  with exn -> close_out oc; raise exn

and collect_component_elements cpnt_desc =
  let ctx =
    {path = cpnt_desc.Instantiation.component_path;
     location = cpnt_desc.Instantiation.component_location;
     instance_nature =
       Instantiation.ComponentElement cpnt_desc.Instantiation.class_name} in
  match evaluate cpnt_desc.Instantiation.component_nature with
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx })
    | Instantiation.Instance inst -> collect_instance_elements ctx inst
    | Instantiation.PredefinedTypeInstance _ ->
        {
          variables = [cpnt_desc];
          equations = []
        }
    | Instantiation.StaticArray cpnt_descs ->
        collect_array_elements cpnt_descs

and collect_instance_elements ctx inst =
  let elts = evaluate inst.Instantiation.elements in
  let named_elts = elts.Instantiation.named_elements
  and unnamed_elts = elts.Instantiation.unnamed_elements in
  let flat_inst = collect_instance_named_elements named_elts
  and equs = List.flatten (List.map (collect_equations ctx) unnamed_elts) in
  { flat_inst with
    equations = flat_inst.equations @ equs
  }

and collect_instance_named_elements named_elts =
  let collect_instance_named_elements' flat_inst (_, elt_desc) =
    let elt_nat = evaluate elt_desc.Instantiation.element_nature in
    match elt_nat with
      | Instantiation.Class _ -> flat_inst
      | Instantiation.Component cpnt_desc ->
          let flat_inst' = collect_component_elements cpnt_desc in
          {
            variables = flat_inst.variables @ flat_inst'.variables;
            equations = flat_inst.equations @ flat_inst'.equations
          } in
  let flat_inst =
    {
      variables = [];
      equations = []
    } in
  List.fold_left collect_instance_named_elements' flat_inst named_elts

and collect_equations ctx = function
  | Instantiation.EquationClause (NameResolve.Permanent, equs) ->
      expand_equations ctx (evaluate equs)
  | Instantiation.EquationClause (NameResolve.Initial, _) ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_InitialEquationClause"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.AlgorithmClause _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_AlgorithmClause"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and expand_equations ctx equs =
  let expand_equation equ = equ.Instantiation.nature in
  let add_connection (expr, sign) (expr', sign') cnect_sets =
    let contains_at_least_one_node_to_connect cnect_set =
      List.mem_assoc expr cnect_set || List.mem_assoc expr' cnect_set in
    let cnect_sets, cnect_sets' =
      List.partition contains_at_least_one_node_to_connect cnect_sets in
    match cnect_sets with
      | [] -> [(expr, sign); (expr', sign')] :: cnect_sets'
      | [cnect_set; cnect_set'] -> (cnect_set @ cnect_set') :: cnect_sets'
      | [cnect_set] when List.mem_assoc expr cnect_set ->
          ((expr', sign') :: cnect_set) :: cnect_sets'
      | [cnect_set] -> ((expr, sign) :: cnect_set) :: cnect_sets'
      | _ :: _ :: _ :: _ -> assert false in
  let expand_connection cnect_sets = function
    | Instantiation.ConnectFlows (sign, expr, sign', expr') ->
        add_connection (expr, sign) (expr', sign') cnect_sets
    | Instantiation.Equal _ | Instantiation.ConditionalEquationE _ |
      Instantiation.WhenClauseE _ -> cnect_sets in
  let generate_flow_equation cnect_set =
    let to_expression (expr, sign) = match sign with
      | NameResolve.Positive -> expr
      | NameResolve.Negative ->
          Instantiation.UnaryOperation (Instantiation.UnaryMinus, expr) in
    let add_expressions expr expr' =
      Instantiation.BinaryOperation (Instantiation.Plus, expr, expr') in
    let exprs = List.map to_expression cnect_set in
    let lhs = List.fold_left add_expressions (Instantiation.Real 0.) exprs in
    Instantiation.Equal (lhs, Instantiation.Real 0.) in
  let collect_equation equs equ = match equ with
    | Instantiation.ConnectFlows _ -> equs
    | Instantiation.Equal _ | Instantiation.ConditionalEquationE _ |
      Instantiation.WhenClauseE _ -> equ :: equs in
  let equ_descs = List.flatten (List.map expand_equation equs) in
  let cnect_sets = List.fold_left expand_connection [] equ_descs
  and equs = List.fold_left collect_equation [] equ_descs in
  let equs' = List.map generate_flow_equation cnect_sets in
  equs @ equs'

and collect_array_elements cpnt_descs =
  let rec collect_array_elements' flat_inst i =
    if i = Array.length cpnt_descs then flat_inst
    else
      let flat_inst' = collect_component_elements cpnt_descs.(i) in
      let flat_inst =
        {
          variables = flat_inst.variables @ flat_inst'.variables;
          equations = flat_inst.equations @ flat_inst'.equations
        } in
      collect_array_elements' flat_inst (i + 1) in
  let flat_inst =
    {
      variables = [];
      equations = []
    } in
  collect_array_elements' flat_inst 0

and generate_flatten_instance ctx fun_defs oc id flat_inst =
  Printf.fprintf oc "class %s\n" id;
  List.iter (generate_variable_declaration ctx oc) flat_inst.variables;
  Printf.fprintf oc "equation\n";
  List.iter (generate_declaration_equation ctx oc) flat_inst.variables;
  generate_equation_descriptions ctx fun_defs oc flat_inst.equations;
  Printf.fprintf oc "end %s;\n" id

and generate_variable_declaration ctx oc cpnt_desc =
  let is_fixed cpnt_nat = match evaluate cpnt_nat with
    | Instantiation.PredefinedTypeInstance predef
      when List.mem_assoc "fixed" predef.Instantiation.attributes ->
        let attr_val = List.assoc "fixed" predef.Instantiation.attributes in
        begin match evaluate attr_val with
          | Instantiation.False -> false
          | _ -> true
        end
    | _ -> true in
  match cpnt_desc.Instantiation.variability with
    | Types.Constant -> ()
    | Types.Parameter when is_fixed cpnt_desc.Instantiation.component_nature ->
        ()
    | Types.Parameter | Types.Discrete | Types.Continuous ->
        Printf.fprintf oc "\t";
        generate_variable_variability oc cpnt_desc;
        generate_variable_causality oc cpnt_desc;
        generate_variable_type ctx oc cpnt_desc;
        generate_variable_name oc cpnt_desc;
        generate_variable_start_value ctx oc cpnt_desc;
        generate_initialization ctx oc cpnt_desc;
        generate_comment oc cpnt_desc;
        Printf.fprintf oc ";\n"

and generate_variable_variability oc cpnt_desc =
  let var = cpnt_desc.Instantiation.variability in
  match var with
    | Types.Constant -> Printf.fprintf oc "constant "
    | Types.Parameter -> Printf.fprintf oc "parameter "
    | Types.Discrete -> Printf.fprintf oc "discrete "
    | Types.Continuous -> ()

and generate_variable_causality oc cpnt_desc =
  let inout = cpnt_desc.Instantiation.causality in
  match inout with
    | Types.Input -> Printf.fprintf oc "input "
    | Types.Output -> Printf.fprintf oc "output "
    | Types.Acausal -> ()

and generate_variable_type ctx oc cpnt_desc =
  let generate_variable_type' predef =
    match predef.Instantiation.predefined_type with
      | Instantiation.IntegerType | Instantiation.RealType ->
          Printf.fprintf oc "Real "
      | Instantiation.BooleanType ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_BooleanType"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Instantiation.StringType ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_StringType"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Instantiation.EnumerationType ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented"; "_EnumType"];
              err_info = [];
              err_ctx = ctx }) (*error*) in
  let var_type = evaluate cpnt_desc.Instantiation.component_nature in
  match var_type with
    | Instantiation.PredefinedTypeInstance predef ->
        generate_variable_type' predef
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Instance _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_InstanceType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.StaticArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_StaticArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)

and generate_variable_name oc cpnt_desc =
  let name = string_of_path cpnt_desc.Instantiation.component_path in
  Printf.fprintf oc "`%s`" name

and generate_variable_start_value ctx oc cpnt_desc =
  let generate_start_value attrs =
    try
      let expr = evaluate (List.assoc "start" attrs) in
      Printf.fprintf oc "(start=";
      generate_expression ctx [] oc expr;
      Printf.fprintf oc ")"
    with Not_found -> () in
  let generate_variable_start_value' predef =
    let attrs = predef.Instantiation.attributes in
    generate_start_value attrs in
  let var_type = evaluate cpnt_desc.Instantiation.component_nature in
  match var_type with
    | Instantiation.PredefinedTypeInstance predef ->
        generate_variable_start_value' predef
    | Instantiation.DynamicArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_DynamicArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Instance _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_InstanceType"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.StaticArray _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_StaticArrayType"];
            err_info = [];
            err_ctx = ctx }) (*error*)

and string_of_path = function
  | [] -> assert false
  | [Instantiation.Name id] -> id
  | [Instantiation.Index i] -> Printf.sprintf "[%d]" (i + 1)
  | Instantiation.Name id :: path ->
      Printf.sprintf "%s.%s" id (string_of_path path)
  | Instantiation.Index i :: path ->
      Printf.sprintf "[%d].%s" (i + 1) (string_of_path path)

and generate_initialization ctx oc cpnt_desc =
  let var = cpnt_desc.Instantiation.variability
  and equ = cpnt_desc.Instantiation.declaration_equation in
  match var, equ with
    | Types.Parameter, Some expr ->
        Printf.fprintf oc " = ";
        generate_expression ctx [] oc (evaluate expr)
    | _ -> ()

and generate_comment oc cpnt_desc =
  Printf.fprintf oc " \"%s\"" cpnt_desc.Instantiation.comment

and generate_declaration_equation ctx oc cpnt_desc =
  let name = string_of_path cpnt_desc.Instantiation.component_path
  and var = cpnt_desc.Instantiation.variability
  and equ = cpnt_desc.Instantiation.declaration_equation in
  match var, equ with
    | (Types.Continuous | Types.Discrete), Some expr ->
        Printf.fprintf oc "\t`%s` = " name;
        generate_expression ctx [] oc (evaluate expr);
        Printf.fprintf oc ";\n"
    | _ -> ()

and generate_equation_descriptions ctx fun_defs oc equ_descs =
  let rec generate_equation = function
    | Instantiation.Equal (expr, expr') ->
        generate_equal expr expr'
    | Instantiation.ConditionalEquationE _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented"; "_ConditionalEqu"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.ConnectFlows _ -> assert false
    | Instantiation.WhenClauseE alts ->
        generate_when_clause alts
  and generate_equal expr expr' =
    Printf.fprintf oc "\t";
    begin match expr with
      | Instantiation.Tuple [] -> ()
      | _ ->
          generate_expression ctx fun_defs oc expr;
          Printf.fprintf oc " = "
    end;
    generate_expression ctx fun_defs oc expr';
    Printf.fprintf oc ";\n"
  and generate_when_clause = function
    | [] -> ()
    | [(expr, equs)] ->
        Printf.fprintf oc "when ";
        generate_expression ctx fun_defs oc expr;
        Printf.fprintf oc " then\n";
        List.iter generate_when_equation equs;
        Printf.fprintf oc "end when;\n"
    | (expr, equs) :: alts ->
        Printf.fprintf oc "when ";
        generate_expression ctx fun_defs oc expr;
        Printf.fprintf oc " then\n";
        List.iter generate_when_equation equs;
        Printf.fprintf oc "else";
        generate_when_clause alts
  and generate_when_equation equ =
    let equ' = equ.Instantiation.nature in
    List.iter generate_equation equ' in
  List.iter generate_equation equ_descs

and generate_expression ctx fun_defs oc = function
  | Instantiation.BinaryOperation (bin_op, expr, expr') ->
      generate_binary_operation ctx fun_defs oc bin_op expr expr'
  | Instantiation.ClassReference cl_def ->
      generate_class_reference fun_defs oc cl_def
  | Instantiation.ComponentReference cpnt_desc ->
      generate_component_reference fun_defs oc cpnt_desc
  | Instantiation.EnumerationElement _ ->
      raise (GenericError
        { err_msg = [ "_NotYetImplemented";
                      "_ExprOfType";
                      "enumeration" ];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.False ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_Expr"; "false"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.FieldAccess _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_FieldAccessExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.FunctionCall (expr, exprs) ->
      generate_function_call ctx fun_defs oc expr exprs
  | Instantiation.If (alts, expr) -> generate_if ctx fun_defs oc alts expr
  | Instantiation.IndexedAccess _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_IndexedAccessExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Integer i -> Printf.fprintf oc "%ld" i
  | Instantiation.LoopVariable _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_LoopVar"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.NoEvent expr -> generate_no_event ctx fun_defs oc expr
  | Instantiation.PredefinedIdentifier id -> Printf.fprintf oc "%s" id
  | Instantiation.Range _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_RangeExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Real f ->
      let s = Printf.sprintf "%.16g" f in
      if String.contains s '.' or String.contains s 'e' then
        Printf.fprintf oc "%s" s
      else Printf.fprintf oc "%s." s
  | Instantiation.Record _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_ExprOfType"; "record"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.String _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_ExprOfType"; "String"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.True ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_Expr"; "true"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.Tuple _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_TupleExpr"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Instantiation.UnaryOperation (un_op, expr) ->
      generate_unary_operation ctx fun_defs oc un_op expr
  | Instantiation.Vector exprs ->
      generate_vector ctx fun_defs oc exprs
  | Instantiation.VectorReduction _ ->
      raise (GenericError
        { err_msg = ["_NotYetImplemented"; "_VectorReduct"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and generate_binary_operation ctx fun_defs oc bin_op expr expr' =
  let string_of_binary_operation_kind = function
    | Instantiation.And ->
        raise (GenericError
          { err_msg = [ "_NotYetImplemented";
                        "_BooleanOperator";
                        "and" ];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Divide -> "/"
    | Instantiation.EqualEqual -> "=="
    | Instantiation.GreaterEqual -> ">="
    | Instantiation.Greater -> ">"
    | Instantiation.LessEqual -> "<="
    | Instantiation.Less -> "<"
    | Instantiation.Times -> "*"
    | Instantiation.NotEqual -> "<>"
    | Instantiation.Or ->
        raise (GenericError
          { err_msg = [ "_NotYetImplemented";
                        "_BooleanOperator";
                        "or" ];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.Plus -> "+"
    | Instantiation.Power -> "^"
    | Instantiation.Minus -> "-" in
  Printf.fprintf oc "(";
  generate_expression ctx fun_defs oc expr;
  Printf.fprintf oc " %s " (string_of_binary_operation_kind bin_op);
  generate_expression ctx fun_defs oc expr';
  Printf.fprintf oc ")"

and generate_class_reference fun_defs oc cl_def =
  let rec last = function
    | [] -> assert false
    | [Instantiation.Name id] -> id
    | [Instantiation.Index _] -> assert false
    | _ :: path -> last path in
  let ctx = {path = cl_def.Instantiation.class_path;
             location = cl_def.Instantiation.class_location;
             instance_nature = Instantiation.ClassElement} in
  let function_path_element path_elt = match path_elt with
    | Instantiation.Name _ -> path_elt
    | Instantiation.Index _ -> Instantiation.Index 0 in
  let index path =
    let rec index' i = function
      | [] ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented";
                         "_FuncDefInNonInstantiatedClass"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | (ctx, _) :: _ when ctx.path = path -> i
      | _ :: fun_defs -> index' (i + 1) fun_defs in
    index' 1 fun_defs in
  let generate_external_call ext_call =
    match ext_call.NameResolve.nature with
      | NameResolve.PrimitiveCall "builtin" ->
          let name = last ctx.path in
          Printf.fprintf oc "Modelica.Math.%s" name
      | NameResolve.PrimitiveCall "C" ->
          let path' = List.map function_path_element ctx.path in
          Printf.fprintf oc "Function%d" (index path')
      | NameResolve.PrimitiveCall lang ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented";
                         "_ExternalCallToLanguage";
                         lang];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | NameResolve.ExternalProcedureCall _ ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented";
                         "_ExternalProcCall"];
              err_info = [];
              err_ctx = ctx }) (*error*) in
  let generate_long_dscription long_desc =
    match evaluate long_desc.NameResolve.external_call with
      | None ->
          raise (GenericError
            { err_msg = ["_NotYetImplemented";
                         "_NonExternalCallClassRef"];
              err_info = [];
              err_ctx = ctx }) (*error*)
      | Some ext_call -> generate_external_call ext_call in
  match cl_def.Instantiation.description with
    | Instantiation.ClassDescription (_, cl_desc) ->
        generate_long_dscription cl_desc.Instantiation.long_description
    | Instantiation.PredefinedType _ ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented";
                       "_PredefinedTypeClassRef"];
            err_info = [];
            err_ctx = ctx }) (*error*)

and generate_component_reference fun_defs oc cpnt_desc =
  let name = string_of_path cpnt_desc.Instantiation.component_path in
  Printf.fprintf oc "`%s`" name

and generate_function_call ctx fun_defs oc expr exprs =
  generate_expression ctx fun_defs oc expr;  
  Printf.fprintf oc "(";
  generate_expressions ctx fun_defs oc exprs;
  Printf.fprintf oc ")"

and generate_expressions ctx fun_defs oc = function
  | [] -> ()
  | [expr] -> generate_expression ctx fun_defs oc expr;
  | expr :: exprs ->
      generate_expression ctx fun_defs oc expr;
      Printf.fprintf oc ", ";
      generate_expressions ctx fun_defs oc exprs

and generate_if ctx fun_defs oc alts expr =
  let rec generate_alternatives = function
    | [] -> Printf.fprintf oc " "; generate_expression ctx fun_defs oc expr
    | (expr, expr') :: alts ->
        Printf.fprintf oc "if ";
        generate_expression ctx fun_defs oc expr;
        Printf.fprintf oc " then ";
        generate_expression ctx fun_defs oc expr';
        Printf.fprintf oc " else";
        generate_alternatives alts in
  generate_alternatives alts

and generate_no_event ctx fun_defs oc expr =
  Printf.fprintf oc "noEvent(";
  generate_expression ctx fun_defs oc expr;
  Printf.fprintf oc ")"

and generate_unary_operation ctx fun_defs oc un_op expr =
  let string_of_binary_operation_kind = function
    | Instantiation.Not ->
        raise (GenericError
          { err_msg = ["_NotYetImplemented";
                       "_BooleanOperator";
                       "not"];
            err_info = [];
            err_ctx = ctx }) (*error*)
    | Instantiation.UnaryMinus -> "-" in
  Printf.fprintf oc "(%s " (string_of_binary_operation_kind un_op);
  generate_expression ctx fun_defs oc expr;
  Printf.fprintf oc ")"

and generate_vector ctx fun_defs oc exprs =
  let exprs' = Array.to_list exprs in
  Printf.fprintf oc "{ ";
  generate_expressions ctx fun_defs oc exprs';
  Printf.fprintf oc " }"
