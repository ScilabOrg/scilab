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

type ('a, 'b) node =
  {
    nature: 'a;
    info: 'b
  }

type instance =
  {
    enclosing_instance: instance option;
    kind: Types.kind;
    elements: instance_elements Lazy.t
  }

and instance_elements =
  {
    named_elements: (string * element_description) list;
    unnamed_elements: equation_or_algorithm_clause list
  }

and element_description =
  {
    redeclare: bool;
    element_nature: element_nature Lazy.t
  }

and element_nature =
  | Class of class_definition
  | Component of component_description

and class_definition =
  {
    class_type: Types.class_specifier;
    class_path: path;
    class_flow: bool option;
    class_variability: Types.variability option;
    class_causality: Types.causality option;
    description: description;
    modification: modification_argument list;
    class_location: Parser.location
  }

and path = path_element list

and path_element =
  | Name of string
  | Index of int

and description =
  | ClassDescription of context * class_description
  | PredefinedType of predefined_type

and class_description =
  {
    class_kind: Types.kind;
    class_annotations: annotation list;
    long_description: NameResolve.long_description
  }

and annotation = inverse_function Lazy.t

and inverse_function =
  {
    function_class: class_definition;
    arguments: (string * string) list
  }

and class_modification = (string * modification_argument) list

and modification_argument =
  {
    each: bool;
    action: modification_action
  }

and modification_action =
  | ElementModification of modification
  | ElementRedeclaration of element_description

and modification =
  | Modification of class_modification * expression Lazy.t option
  | Assignment of expression Lazy.t
  | Equality of expression Lazy.t

and component_description =
  {
    component_path: path;
    flow: bool;
    variability: Types.variability;
    causality: Types.causality;
    component_nature: component_nature Lazy.t;
    declaration_equation: expression Lazy.t option;
    comment: string;
    component_location: Parser.location;
    class_name: string
  }

and component_nature =
  | DynamicArray of component_description
      (* one representative member of the collection *)
  | Instance of instance
  | PredefinedTypeInstance of predefined_type_instance
  | StaticArray of component_description array

and predefined_type_instance =
  {
    predefined_type: predefined_type;
    attributes: (string * expression Lazy.t) list
  }

and predefined_type =
  | BooleanType
  | IntegerType
  | RealType
  | StringType
  | EnumerationType

and equation_or_algorithm_clause =
  | EquationClause of NameResolve.validity * equation list Lazy.t
  | AlgorithmClause of NameResolve.validity * algorithm list Lazy.t

and validity = Initial | Permanent

and equation = (equation_desc list, NameResolve.equation) node

and equation_desc =
  | Equal of expression * expression
  | ConditionalEquationE of (expression * equation list) list *
      equation list
  | ConnectFlows of NameResolve.sign * expression *
      NameResolve.sign * expression
  | WhenClauseE of (expression * equation list) list

and algorithm = (algorithm_desc list, NameResolve.algorithm) node

and algorithm_desc =
  | Assign of expression * expression
  | FunctionCallA of expression * expression list
  | MultipleAssign of expression list * expression * expression list
  | Break
  | Return
  | ConditionalEquationA of (expression * algorithm list) list *
      algorithm list
  | ForClauseA of expression (* range *) * algorithm list
  | WhileClause of expression * algorithm list
  | WhenClauseA of (expression * algorithm list) list

and expression =
  | BinaryOperation of binary_operator_kind * expression * expression
  | ClassReference of class_definition
  | ComponentReference of component_description
  | EnumerationElement of string
  | False
  | FieldAccess of expression * string
  | FunctionCall of expression * expression list
  | If of (expression (* condition *) * expression) list *
      expression (* default *)
  | IndexedAccess of expression * expression list (* subscripts *)
  | Integer of int32
  | LoopVariable of int (* number of nested for loops to skip *)
  | NoEvent of expression
  | PredefinedIdentifier of string
  | Range of expression * expression * expression
  | Real of float
  | Record of (string * expression) list
  | String of string
  | True
  | Tuple of expression list
  | UnaryOperation of unary_operator_kind * expression
  | Vector of expression array
  | VectorReduction of expression list (* ranges *) * expression

and unary_operator_kind =
  | Not
  | UnaryMinus

and binary_operator_kind =
  | And
  | Divide
  | EqualEqual
  | GreaterEqual
  | Greater
  | LessEqual
  | Less
  | Times
  | NotEqual
  | Or
  | Plus
  | Power
  | Minus

and context =
  {
    toplevel: (string * element_description) list Lazy.t;
    path: path;
    context_flow: bool option;
    context_variability: Types.variability option;
    context_causality: Types.causality option;
    parent_context: context option; (* for normal parent scope lookup *)
    class_context: context_nature; (* for normal (class-based) lookup *)
    instance_context: instance option; (* for dynamically scoped identifiers *)
    location: Parser.location;
    instance_nature: instance_nature
  }

and context_nature =
  | ToplevelContext
  | InstanceContext of instance
  | ForContext of context *
      expression option (* current value of the loop variable, if available *)
  | FunctionEvaluationContext of context * expression * expression list

(** Error description *)
and error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: context
  }

and instance_nature =
  | ClassElement
  | ComponentElement of string

exception InstantError of error_description


(* Utilities *)

let levels = ref 0

let spaces () = for i = 1 to !levels do Printf.printf "  " done

let nest i =
  spaces (); Printf.printf "ForContext %ld\n" i;
  incr levels

let nest2 i =
  spaces (); Printf.printf "ReductionContext %ld\n" i;
  incr levels

let unnest () =
  decr levels;
  spaces (); Printf.printf "Leaving ForContext\n"

let evaluate x = Lazy.force x

module ArrayExt =
  struct
    let map2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.map2"
      else begin
        let create_array i = f a.(i) a'.(i) in
        Array.init l create_array
      end
    let for_all2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.for_all2"
      else begin
        let rec for_all2' i =
          i = l || (f a.(i) a'.(i) && for_all2' (i + 1)) in
        for_all2' 0
      end
    let exists2 f a a' =
      let l = Array.length a
      and l' = Array.length a' in
      if l <> l' then invalid_arg "ArrayExt.exists2"
      else begin
        let rec exists2' i =
          i < l && (f a.(i) a'.(i) || exists2' (i + 1)) in
        exists2' 0
      end
  end


(* Instantiation functions *)

let rec evaluate_toplevel_definitions dic defs =
  let rec ctx =
    {
      toplevel = lazy (dic @ evaluate defs');
      path = [];
      context_flow = None;
      context_variability = None;
      context_causality = None;
      parent_context = None;
      class_context = ToplevelContext;
      instance_context = None;
      location = {Parser.start = 0; Parser.enddd = 0; filename = Parser.CommandLine};
      instance_nature = ClassElement
    }
  and defs' = lazy (List.map (evaluate_toplevel_definition ctx) defs) in
  evaluate defs'

and evaluate_toplevel_definition ctx (id, elt_desc) =
  let elt_loc = [Name id] in
  let ctx = {ctx with
               path = elt_loc;
               location = elt_desc.NameResolve.element_location;
               instance_nature = instance_nature_of_element elt_desc} in
  let elt_nat = elt_desc.NameResolve.element_nature in
  let elt_desc' =
    {
      redeclare = false;
      element_nature = lazy (evaluate_toplevel_element ctx elt_loc elt_nat)
    } in
  id, elt_desc'

and evaluate_toplevel_element ctx elt_loc = function
  | NameResolve.Component cpnt_desc ->
      let cpnt_desc' =
        instantiate_component_description ctx [] None elt_loc cpnt_desc in
      Component cpnt_desc'
  | NameResolve.Class cl_def ->
      let cl_def' = evaluate_class_definition ctx [] elt_loc cl_def in
      Class cl_def'
  | NameResolve.ComponentType _ ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_ComponentTypeElemInstant"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | NameResolve.PredefinedType _ ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_PredefinedTypeElemInstant"];
          err_info = [];
          err_ctx = ctx }) (*error*)

and instantiate_class_description ctx modifs rhs elt_loc cl_desc =
  let elements inst =
    let ctx' =
      { ctx with
        toplevel = lazy (evaluate ctx.toplevel);
        path = elt_loc;
        parent_context = Some ctx;
        class_context = InstanceContext inst;
        instance_context = None
      } in
    instantiate_class_elements ctx' modifs rhs cl_desc.long_description in
  let rec inst =
    {
      enclosing_instance = enclosing_instance ctx;
      kind = cl_desc.class_kind;
      elements = lazy (elements inst)
    } in
  inst

and enclosing_instance ctx = match ctx.class_context with
  | ToplevelContext -> None
  | InstanceContext inst -> Some inst
  | ForContext (ctx', _) | FunctionEvaluationContext (ctx', _, _) ->
      enclosing_instance ctx'

and instantiate_class_elements ctx modifs rhs long_desc =
  let rec merge_elements named_elts unnamed_elts = function
    | [] ->
        {
          named_elements = named_elts;
          unnamed_elements = unnamed_elts
        }
    | inherited_elts :: inherited_eltss ->
        let named_elts' = named_elts @ inherited_elts.named_elements
        and unnamed_elts' = unnamed_elts @ inherited_elts.unnamed_elements in
        merge_elements named_elts' unnamed_elts' inherited_eltss in
  let named_elts = long_desc.NameResolve.named_elements
  and unnamed_elts = long_desc.NameResolve.unnamed_elements
  and exts = long_desc.NameResolve.extensions in
  let named_elts' = instantiate_local_named_elements ctx modifs rhs named_elts
  and unnamed_elts' = instantiate_local_unnamed_elements ctx unnamed_elts
  and inherited_eltss = instantiate_inherited_elements ctx modifs rhs exts in
  merge_elements named_elts' unnamed_elts' inherited_eltss

and instantiate_local_named_elements ctx modifs rhs named_elts =
  List.fold_right (instantiate_local_named_element ctx modifs rhs) named_elts []

and instantiate_local_named_element ctx modifs rhs (id, elt_desc) named_elts =
  let rec filter_current_element_modifications = function
    | [] -> []
    | (id', arg) :: modifs when id' = id ->
        arg :: filter_current_element_modifications modifs
    | _ :: modifs -> filter_current_element_modifications modifs
  and select_current_element_value = function
    | None -> None
    | Some expr -> Some (lazy (field_access ctx (evaluate expr) id)) in
  let modifs' = filter_current_element_modifications modifs
  and rhs' = select_current_element_value rhs
  and elt_loc = ctx.path @ [Name id] in
  let ctx = {ctx with
               path = elt_loc;
               location = elt_desc.NameResolve.element_location;
               instance_nature = instance_nature_of_element elt_desc} in
  let elt_nat =
    lazy (instantiate_local_element_nature ctx modifs' rhs' elt_loc elt_desc) in
  let named_elt =
    id,
    {
      redeclare = elt_desc.NameResolve.redeclare;
      element_nature = elt_nat
    } in
  named_elt :: named_elts

and instantiate_local_element_nature ctx modifs rhs elt_loc elt_desc =
  match elt_desc.NameResolve.element_nature with
    | NameResolve.Component cpnt_desc ->
        let cpnt_desc' =
          instantiate_component_description ctx modifs rhs elt_loc cpnt_desc in
       Component cpnt_desc'
    | NameResolve.Class cl_def ->
        let cl_def' = evaluate_class_definition ctx modifs elt_loc cl_def in
        Class cl_def'
    | NameResolve.ComponentType _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_ComponentTypeElemInstant"];
            err_info = [];
            err_ctx = ctx })
    | NameResolve.PredefinedType _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_PredefinedTypeElemInstant"];
            err_info = [];
            err_ctx = ctx })

and instantiate_component_description ctx modifs rhs elt_loc cpnt_desc =
  let cpnt_type = evaluate cpnt_desc.NameResolve.component_type in
  let flow = evaluate cpnt_type.Types.flow
  and var = evaluate cpnt_type.Types.variability
  and inout = evaluate cpnt_type.Types.causality
  and type_spec = evaluate cpnt_desc.NameResolve.type_specifier
  and dims = evaluate cpnt_desc.NameResolve.dimensions
  and modifs' = match evaluate cpnt_desc.NameResolve.modification with
    | None -> modifs
    | Some modif ->
        let modif' = evaluate_modification ctx modif in
        modifs @ [{ each = false; action = ElementModification modif' }]
  and cmt = cpnt_desc.NameResolve.comment in
  component_nature ctx modifs' rhs elt_loc flow var inout type_spec dims cmt

and component_nature ctx modifs rhs elt_loc flow var inout type_spec dims cmt =
  let type_spec' = evaluate_expression ctx type_spec in
  let ctx = {ctx with location = expression_location ctx type_spec} in
  expand_array ctx modifs rhs elt_loc flow var inout type_spec' dims cmt

and expand_array ctx modifs rhs elt_loc flow var inout type_spec dims cmt =
  let rec expand_along_dimension dim dims = match dim with
    | NameResolve.Colon -> expand_dynamic_array dims
    | NameResolve.Expression expr ->
        let expr' = evaluate_expression ctx expr in
        expand_static_array dims expr' expr
  and expand_dynamic_array dims =
    (* No need to select modifications since all of them have 'each' set *)
    let elt_loc' = elt_loc @ [Index 0] in
    let ctx = { ctx with path = elt_loc' } in
    let expr =
      expand_array ctx modifs rhs elt_loc' flow var inout type_spec dims cmt in
    DynamicArray expr
  and expand_static_array dims expr' expr =
    let ctx = {ctx with location = expression_location ctx expr} in
    let expand_element i =
      let rec select_subargument arg = match arg.each with
        | true -> arg
        | false -> { arg with action = select_subarray arg.action }
      and select_subarray arg = match arg with
	| ElementModification modif ->
            ElementModification (select_submodification modif)
        | ElementRedeclaration _ -> arg
      and select_sub_class_modification_element (id, arg) =
        id, select_subargument arg
      and select_submodification = function
        | Modification (modifs, rhs) ->
            let modifs' = List.map select_sub_class_modification_element modifs
            and rhs' = select_rhs_subarray rhs in
            Modification (modifs', rhs')
        | Assignment expr ->
            let expr' = lazy (select_row i (evaluate expr)) in
            Assignment expr'
        | Equality expr ->
            let expr' = lazy (select_row i (evaluate expr)) in
            Equality expr'
      and select_rhs_subarray = function
        | None -> None
        | Some expr -> Some (lazy (select_row i (evaluate expr)))
      and select_row i = function
	| Vector exprs -> exprs.(i)
        | expr ->
            IndexedAccess (expr, [Integer (Int32.succ (Int32.of_int i))]) in
      let modifs = List.map select_subargument modifs
      and rhs = select_rhs_subarray rhs
      and elt_loc = elt_loc @ [Index i] in
      expand_array ctx modifs rhs elt_loc flow var inout type_spec dims cmt in
    match expr' with
      | Integer i ->
          let a = Array.init (Int32.to_int i) expand_element in
          StaticArray a
      | _ ->
          raise (InstantError
          { err_msg = ["_NonIntegerArrayDim"];
            err_info = [];
            err_ctx = ctx }) (*error*) in
  match dims with
    | [] ->
        let cl_def = class_definition_of_type_specification ctx type_spec in
        create_instance ctx modifs rhs elt_loc flow var inout cl_def cmt
    | dim :: dims ->
        {
          component_path = elt_loc;
          flow = flow;
          variability = var;
          causality = inout;
          component_nature = lazy (expand_along_dimension dim dims);
          declaration_equation = rhs;
          comment = cmt;
          component_location = ctx.location;
          class_name = instance_class_name ctx.instance_nature
        }

and create_instance ctx modifs rhs elt_loc flow var inout cl_def cmt =
  let merge_class_modifications arg modifs = match arg.action with
    | ElementModification (Modification (modifs', _)) -> modifs' @ modifs
    | ElementModification (Assignment _ | Equality _) -> modifs
    | ElementRedeclaration _ -> modifs in
  let rec declaration_equation modifs rhs =
    let rec declaration_equation' = function
      | [] -> None
      | {
          action =
            ElementModification (
              Modification (_, Some expr) | Assignment expr | Equality expr)
        } :: _ -> Some expr
      | _ :: args -> declaration_equation' args in
    match rhs with
      | None -> declaration_equation' modifs
      | Some _ -> rhs in
  let flow' = match cl_def.class_flow, ctx.context_flow with
    | None, None -> flow
    | Some flow', None | None, Some flow' -> flow || flow'
    | Some flow', Some flow'' -> flow || flow' || flow''
  and var' = match cl_def.class_variability, ctx.context_variability with
    | None, None -> var
    | Some var', None | None, Some var' -> Types.min_variability var var'
    | Some var', Some var'' ->
        Types.min_variability var (Types.min_variability var' var'')
  and inout' = match inout, cl_def.class_causality with
    | Types.Input, _ | _, Some Types.Input -> Types.Input
    | Types.Output, _ | _, Some Types.Output -> Types.Output
    | _ -> Types.Acausal in
  let modifs' =
    List.fold_right
      merge_class_modifications
      (modifs @ cl_def.modification)
      []
  and rhs' = declaration_equation modifs rhs in
  match cl_def.description with
    | ClassDescription (ctx', cl_desc) ->
        let class_name = instance_class_name ctx.instance_nature in
        let ctx' =
          { ctx' with
            context_flow = Some flow';
            context_variability = Some var';
            context_causality = Some inout';
            instance_context = enclosing_instance ctx;
            instance_nature = ComponentElement class_name
          } in
        {
          component_path = elt_loc;
          flow = flow';
          variability = var';
          causality = inout';
          component_nature =
            lazy (create_class_instance ctx' modifs' rhs' elt_loc cl_desc);
          declaration_equation = rhs';
          comment = cmt;
          component_location = ctx'.location;
          class_name = class_name
        }
    | PredefinedType predef ->
        let class_name = instance_class_name ctx.instance_nature in
        let ctx' =
          { ctx with
            context_flow = Some flow';
            context_variability = Some var';
            context_causality = Some inout';
            instance_nature = ComponentElement class_name
          } in
        {
          component_path = elt_loc;
          flow = flow';
          variability = var';
          causality = inout';
          component_nature =
            lazy (create_predefined_type_instance ctx' modifs' predef);
          declaration_equation = rhs';
          comment = cmt;
          component_location = ctx'.location;
          class_name = class_name
        }

and create_temporary_instance ctx cl_def =
  match cl_def.description with
    | ClassDescription (ctx', cl_desc) ->
        {
          component_path = [];
          flow = false;
          variability = Types.Continuous;
          causality = Types.Acausal;
          component_nature =
            lazy (create_class_instance ctx' [] None [] cl_desc);
          declaration_equation = None;
          comment = "";
          component_location = ctx'.location;
          class_name = instance_class_name ctx.instance_nature
        }
    | PredefinedType predef -> assert false (*error*)

and class_definition_of_type_specification ctx type_spec =
  let predefined_class_specifier = function
    | "Boolean" -> Types.boolean_class_type
    | "Integer" -> Types.integer_class_type
    | "Real" -> Types.real_class_type
    | "String" -> Types.string_class_type
    | s ->
        raise (InstantError
          { err_msg = ["_UnknownIdentifier"; s];
            err_info = [];
            err_ctx = ctx }) (*error*)
  and predefined_class_description = function
    | "Boolean" -> PredefinedType BooleanType
    | "Integer" -> PredefinedType IntegerType
    | "Real" -> PredefinedType RealType
    | "String" -> PredefinedType StringType
    | s ->
        raise (InstantError
          { err_msg = ["_UnknownIdentifier"; s];
            err_info = [];
            err_ctx = ctx }) (*error*) in
  match type_spec with
    | ClassReference cl_def -> cl_def
    | PredefinedIdentifier id ->
        {
          class_type = predefined_class_specifier id;
          class_path = [Name id];
          class_flow = None;
          class_variability = None;
          class_causality = None;
          description = predefined_class_description id;
          modification = [];
          class_location = ctx.location
        }
    | _ -> assert false (*error*)

and create_class_instance ctx modifs rhs elt_loc cl_desc =
  let inst = instantiate_class_description ctx modifs rhs elt_loc cl_desc in
  Instance inst

and create_predefined_type_instance ctx modifs predef =
  let inst =
    {
      predefined_type = predef;
      attributes = predefined_type_attributes ctx modifs
    } in
  PredefinedTypeInstance inst

and predefined_type_attributes ctx modifs =
  let rec predefined_type_attributes attrs = function
    | [] -> attrs
    | (id, { action = ElementModification (Equality expr) }) :: modifs
      when not (List.mem_assoc id attrs) ->
        let attrs' = (id, expr) :: attrs in
        predefined_type_attributes attrs' modifs
    | _ :: modifs -> predefined_type_attributes attrs modifs in
  predefined_type_attributes [] modifs

and instantiate_inherited_elements ctx modifs rhs exts =
  List.fold_right (instantiate_inherited_element ctx modifs rhs) exts []

and instantiate_inherited_element ctx modifs rhs (_, modif_cl) inherited_elts =
  let instantiate_inherited_element' modifs cl_def =
    match cl_def.description with
      | ClassDescription (ctx', cl_desc) ->
          let ctx' = { ctx with parent_context = Some ctx' } in
          let long_desc = cl_desc.long_description in
          instantiate_class_elements ctx' modifs rhs long_desc
      | PredefinedType _ -> assert false (*error*) in
  let type_spec = evaluate modif_cl.NameResolve.base_class
  and modifs' = evaluate modif_cl.NameResolve.class_modification in
  let type_spec' = evaluate_expression ctx type_spec
  and ctx = {ctx with location = expression_location ctx type_spec} in
  let modifs = modifs @ evaluate_class_modification ctx modifs' in
  match type_spec' with
    | ClassReference cl_def ->
        instantiate_inherited_element' modifs cl_def :: inherited_elts
    | _ -> assert false (*error*)
 
and evaluate_class_definition ctx modifs elt_loc cl_def =
  match evaluate cl_def.NameResolve.description with
    | NameResolve.LongDescription long_desc ->
        let cl_anns =
          evaluate_class_annotations
            ctx
            long_desc.NameResolve.class_annotations in
        let cl_def' =
          {
            class_kind = Types.Class;
            class_annotations = cl_anns;
            long_description = long_desc
          } in
        {
          class_type = evaluate cl_def.NameResolve.class_type;
          class_path = elt_loc;
          class_flow = None;
          class_variability = None;
          class_causality = None;
          description = ClassDescription (ctx, cl_def');
          modification = modifs;
          class_location = ctx.location
        }
    | NameResolve.ShortDescription short_desc ->
        raise (InstantError
          {err_msg = ["_NotYetImplemented"; "_ShortClassDef"];
           err_info = [];
           err_ctx = {ctx with path = elt_loc;
                      instance_nature = ClassElement}})

and evaluate_class_annotations ctx cl_anns =
  let evaluate_class_annotation' cl_ann =
    let inv_func = evaluate cl_ann in
    let expr =
      evaluate_expression ctx inv_func.NameResolve.function_class in
    match expr with
    | ClassReference cl_def ->
        {
          function_class = cl_def;
          arguments = inv_func.NameResolve.arguments
        }
    | _ -> assert false (*error*) in
  let evaluate_class_annotation cl_ann =
    lazy (evaluate_class_annotation' cl_ann) in
  List.map evaluate_class_annotation cl_anns

and evaluate_class_modification ctx cl_modif =
  let add_modification_argument arg cl_modif' =
    match arg.NameResolve.action with
      | None -> cl_modif'
      | Some modif ->
          let arg' =
            arg.NameResolve.target,
            {
              each = arg.NameResolve.each;
              action = evaluate_modification_action ctx modif
            } in
          arg' :: cl_modif' in
  List.fold_right add_modification_argument cl_modif []

and evaluate_modification_action ctx = function
  | NameResolve.ElementModification modif ->
      let modif' = evaluate_modification ctx modif in
      ElementModification modif'
  | NameResolve.ElementRedeclaration elt_desc ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_ElementRedeclaration"];
          err_info = [];
          err_ctx = ctx })

and evaluate_modification ctx = function
  | NameResolve.Modification (modifs, rhs) ->
      let modifs' = evaluate_class_modification ctx modifs
      and rhs' = evaluate_modification_expression ctx rhs in
      Modification (modifs', rhs')
  | NameResolve.Assignment expr ->
      let expr = evaluate expr in
      let ctx = {ctx with location = expression_location ctx expr} in
      raise (InstantError
        { err_msg = ["_NotYetImplemented"; "_AssignExprInElemModif"];
          err_info = [];
          err_ctx = ctx })
  | NameResolve.Equality expr ->
      let expr' = lazy (evaluate_expression ctx (evaluate expr)) in
      Equality expr'

and evaluate_modification_expression ctx = function
  | None -> None
  | Some expr ->
      let expr' = lazy (evaluate_expression ctx (evaluate expr)) in
      Some expr'

and instantiate_local_unnamed_elements ctx unnamed_elts =
  List.map (instantiate_local_unnamed_element ctx) (evaluate unnamed_elts)

and instantiate_local_unnamed_element ctx unnamed_elt =
  match unnamed_elt with
    | NameResolve.EquationClause (validity, equs) ->
        EquationClause (validity, lazy (instantiate_equations ctx equs))
    | NameResolve.AlgorithmClause (validity, algs) ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_AlgoClause"];
            err_info = [];
            err_ctx = ctx })

and instantiate_equations ctx equs =
  let instantiate_equations' equ equs =
    let equs' =  instantiate_equation ctx equ in
    { nature = equs'; info = equ } :: equs in
  List.fold_right instantiate_equations' equs []

and instantiate_equation ctx equ = match equ.NameResolve.nature with
  | NameResolve.Equal (expr, expr') -> instantiate_equal ctx expr expr'
  | NameResolve.ConditionalEquationE (alts, default) ->
      instantiate_conditional_equation ctx alts default
  | NameResolve.ForClauseE (ranges, equs) ->
      instantiate_for_clause_e ctx ranges equs
  | NameResolve.ConnectFlows (sign, expr, sign', expr') ->
      instantiate_connection ctx sign expr sign' expr'
  | NameResolve.WhenClauseE alts ->
      instantiate_when_clause_e ctx alts

and instantiate_equal ctx expr expr' =
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  [Equal (expr, expr')]

and instantiate_conditional_equation ctx alts default =
  let rec instantiate_alternatives acc = function
    | [] -> instantiate_default acc default
    | (cond, equs) :: alts -> instantiate_alternative acc cond equs alts
  and instantiate_alternative acc cond equs alts =
    let cond' = evaluate_expression ctx cond in
    match cond' with
      | False -> instantiate_alternatives acc alts
      | True -> instantiate_default acc equs
      | _ ->
          let equs' = instantiate_equations ctx equs in
          instantiate_alternatives ((cond', equs') :: acc) alts
  and instantiate_default acc equs =
    let equs' = instantiate_equations ctx equs in
    [ConditionalEquationE (List.rev acc, equs')] in
  instantiate_alternatives [] alts

and instantiate_when_clause_e ctx alts =
  let instantiate_alternative (cond, equs) =
    let cond' = evaluate_expression ctx cond in
    let equs' = instantiate_equations ctx equs in
    cond', equs' in
  [WhenClauseE (List.map instantiate_alternative alts)]

and instantiate_connection ctx sign expr sign' expr' =
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  [ConnectFlows (sign, expr, sign', expr')]

and instantiate_for_clause_e ctx ranges equs =
  let rec instantiate_for_clause_e' ctx = function
    | [] -> List.flatten (List.map (instantiate_equation ctx) equs)
    | ranges -> equations_of_reduction ctx ranges
  and equations_of_reduction ctx = function
    | Range (Integer start, Integer step, Integer stop) :: ranges ->
        equations_of_range ctx start step stop ranges
    | _ -> assert false (*error*)
  and equations_of_range ctx start step stop ranges =
    let rec equation_list pred start = match pred start with
      | true -> []
      | false ->
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some (Integer start))
            } in
          let equs = instantiate_for_clause_e' ctx' ranges in
          equs @ equation_list pred (Int32.add start step) in
    let cmp = Int32.compare step 0l in
    match cmp with
      | 0 when Int32.compare start stop <> 0 -> assert false (*error*)
      | 0 -> []
      | _ when cmp < 0 ->
          let pred = function i -> Int32.compare i stop < 0 in
          equation_list pred start
      | _ ->
          let pred = function i -> Int32.compare i stop > 0 in
          equation_list pred start in
  let ranges = List.map (evaluate_expression ctx) ranges in
  instantiate_for_clause_e' ctx ranges

and evaluate_expression ctx expr =
  let ctx = {ctx with location = expression_location ctx expr} in
  match expr.NameResolve.nature with
    | NameResolve.BinaryOperation (binop, expr, expr') ->
        evaluate_binary_operation ctx binop expr expr'
    | NameResolve.DynamicIdentifier (level, id) ->
        evaluate_dynamic_identifier ctx level id
    | NameResolve.False -> False
    | NameResolve.FieldAccess (expr, id) ->
        evaluate_field_access ctx expr id
    | NameResolve.FunctionArgument pos -> evaluate_function_argument ctx pos
    | NameResolve.FunctionCall (expr, exprs, expr') ->
        evaluate_function_call ctx expr exprs expr'
    | NameResolve.FunctionInvocation exprs ->
        evaluate_function_invocation ctx exprs
    | NameResolve.If (alts, default) -> evaluate_if ctx alts default
    | NameResolve.IndexedAccess (expr, exprs) ->
        evaluate_indexed_access ctx expr exprs
    | NameResolve.Integer i -> Integer i
    | NameResolve.LocalIdentifier (level, id) ->
        evaluate_local_identifier ctx level id
    | NameResolve.LoopVariable level -> evaluate_loop_variable ctx level
    | NameResolve.NoEvent expr -> evaluate_no_event ctx expr
    | NameResolve.PredefinedIdentifier id -> PredefinedIdentifier id
    | NameResolve.Range (start, step, stop) ->
        evaluate_range ctx start step stop
    | NameResolve.Real f -> Real f
    | NameResolve.String s -> String s
    | NameResolve.ToplevelIdentifier id ->
        evaluate_toplevel_identifier ctx id
    | NameResolve.True -> True
    | NameResolve.Tuple exprs -> evaluate_tuple ctx exprs
    | NameResolve.UnaryOperation (unop, expr) ->
        evaluate_unary_operation ctx unop expr
    | NameResolve.VectorReduction (ranges, expr) ->
        evaluate_vector_reduction ctx ranges expr
    | NameResolve.Vector exprs -> evaluate_vector ctx exprs

and evaluate_binary_operation ctx binop expr expr' =
  let expr = evaluate_expression ctx expr
  and expr' = evaluate_expression ctx expr' in
  match binop with
    | NameResolve.And -> evaluate_and expr expr'
    | NameResolve.Divide -> evaluate_divide expr expr'
    | NameResolve.EqualEqual -> evaluate_equalequal expr expr'
    | NameResolve.GreaterEqual -> evaluate_greater_equal expr expr'
    | NameResolve.Greater -> evaluate_greater expr expr'
    | NameResolve.LessEqual -> evaluate_less_equal expr expr'
    | NameResolve.Less -> evaluate_less expr expr'
    | NameResolve.Times -> evaluate_times expr expr'
    | NameResolve.NotEqual -> evaluate_not_equal expr expr'
    | NameResolve.Or -> evaluate_or expr expr'
    | NameResolve.Plus -> evaluate_plus expr expr'
    | NameResolve.Power -> evaluate_power ctx expr expr'
    | NameResolve.Minus -> evaluate_minus expr expr'

and evaluate_dynamic_identifier ctx level id =
  let rec evaluate_dynamic_identifier' inst level =
    match level, inst.enclosing_instance with
    | 0, _ -> instance_field_access ctx inst id
    | _, Some inst -> evaluate_dynamic_identifier' inst (level - 1)
    | _, None -> assert false (*error*) in
  match ctx.instance_context with
    | Some inst -> evaluate_dynamic_identifier' inst level
    | None -> assert false (*error*)

and evaluate_field_access ctx expr id =
  let expr = evaluate_expression ctx expr in
  field_access ctx expr id

and evaluate_function_argument ctx pos = match ctx.class_context with
  | FunctionEvaluationContext (_, expr, _) when pos = 0 -> expr
  | FunctionEvaluationContext (_, _, exprs) -> List.nth exprs (pos - 1)
  | ForContext (ctx', _) -> evaluate_function_argument ctx' pos
  | InstanceContext _ | ToplevelContext -> assert false (*error*)

and evaluate_function_call ctx expr exprs expr' =
  let expr = evaluate_expression ctx expr
  and exprs = List.map (evaluate_expression ctx) exprs in
  let ctx' =
    { ctx with
      class_context = FunctionEvaluationContext (ctx, expr, exprs)
    } in
  evaluate_expression ctx' expr'

and evaluate_function_invocation ctx exprs =
  let exprs = List.map (evaluate_expression ctx) exprs in
  let evaluate_function_with_arguments = function
    | ClassReference cl_def ->
        evaluate_class_function_invocation cl_def exprs
    | PredefinedIdentifier s ->
        evaluate_predefined_function_invocation ctx s exprs
    | ComponentReference _ ->
        raise (InstantError
          { err_msg = ["_NotYetImplemented"; "_ComponentFuncInvocation"];
            err_info = [];
            err_ctx = ctx })
    | _ -> assert false (*error*) in
  let rec evaluate_function_invocation' ctx = match ctx.class_context with
    | FunctionEvaluationContext (_, expr, _) ->
        evaluate_function_with_arguments expr
    | ForContext (ctx', _) -> evaluate_function_invocation' ctx'
    | InstanceContext _ | ToplevelContext -> assert false (*error*) in
  evaluate_function_invocation' ctx

and evaluate_if ctx alts default =
  let create_if alts default = match alts with
    | [] -> default
    | _ :: _ -> If (alts, default) in
  let rec evaluate_alternatives alts' alts = match alts with
    | [] ->
        let default = evaluate_expression ctx default in
        create_if (List.rev alts') default
    | (expr, expr') :: alts ->
        let expr = evaluate_expression ctx expr in
        evaluate_alternative expr expr' alts' alts
  and evaluate_alternative expr expr' alts' alts = match expr with
    | True ->
        let default = evaluate_expression ctx expr' in
        create_if (List.rev alts') default
    | False -> evaluate_alternatives alts' alts
    | _ ->
        let expr' = evaluate_expression ctx expr' in
        evaluate_alternatives ((expr, expr') :: alts') alts in
  evaluate_alternatives [] alts

and evaluate_indexed_access ctx expr exprs =
  let rec evaluate_indexed_access' expr exprs =
    let rec vector_indexed_access exprs' exprs = match exprs with
      | [] -> expr
      | Integer i :: exprs ->
          let expr' = exprs'.(Int32.to_int i - 1) in
          evaluate_indexed_access' expr' exprs
      | _ -> IndexedAccess (expr, exprs)
    and component_indexed_access cpnt_desc exprs =
      let static_array_indexed_access cpnt_descs exprs = match exprs with
        | [] -> expr
        | Integer i :: exprs ->
            let cpnt_desc = cpnt_descs.(Int32.to_int i - 1) in
            let expr' = ComponentReference cpnt_desc in
            evaluate_indexed_access' expr' exprs
        | exprs -> IndexedAccess (expr, exprs) in
      match evaluate cpnt_desc.component_nature with
        | DynamicArray _ -> IndexedAccess (ComponentReference cpnt_desc, exprs)
        | StaticArray cpnt_descs ->
            static_array_indexed_access cpnt_descs exprs
        | Instance _ | PredefinedTypeInstance _ -> expr in
    match expr, exprs with
      | _, [] -> expr
      | ComponentReference cpnt_desc, _ ->
          component_indexed_access cpnt_desc exprs
      | Vector exprs', _ -> vector_indexed_access exprs' exprs
      | _ -> IndexedAccess (expr, exprs) in
  let expr = evaluate_expression ctx expr
  and exprs = List.map (evaluate_expression ctx) exprs in
  evaluate_indexed_access' expr exprs

and evaluate_local_identifier ctx level id =
  let rec evaluate_local_identifier' ctx inst level =
    match level, ctx.parent_context with
      | 0, _ -> instance_field_access ctx inst id
      | _, Some ctx -> evaluate_local_identifier ctx (level - 1) id
      | _, None -> assert false (*error*) in
  match ctx.class_context with
    | ForContext (ctx, _) | FunctionEvaluationContext (ctx, _, _) ->
        evaluate_local_identifier ctx level id
    | InstanceContext inst -> evaluate_local_identifier' ctx inst level
    | ToplevelContext -> assert false (*error*)

and evaluate_loop_variable ctx level =
  let rec evaluate_loop_variable' ctx level' =
    match level', ctx.class_context with
      | 0, ForContext (_, None) -> LoopVariable level'
      | 0, ForContext (_, Some expr) -> expr
      | _, ForContext (ctx, _) -> evaluate_loop_variable ctx (level' - 1)
      | _, FunctionEvaluationContext (ctx, _, _) ->
          evaluate_loop_variable' ctx level'
      | _, (InstanceContext _ | ToplevelContext) -> assert false (*error*) in
  evaluate_loop_variable' ctx level

and evaluate_no_event ctx expr =
  let expr = evaluate_expression ctx expr in
  match expr with
    | True | False | Integer _ | Real _ | String _ | EnumerationElement _ ->
        expr
    | _ -> NoEvent expr

and evaluate_range ctx start step stop =
  let start = evaluate_expression ctx start
  and step = evaluate_expression ctx step
  and stop = evaluate_expression ctx stop in
  Range (start, step, stop)

and evaluate_toplevel_identifier ctx id =
  let elt_desc = List.assoc id (evaluate ctx.toplevel) in
  match evaluate elt_desc.element_nature with
    | Class cl_def -> ClassReference cl_def
    | Component cpnt_desc -> ComponentReference cpnt_desc

and evaluate_tuple ctx exprs =
  Tuple (List.map (evaluate_expression ctx) exprs)

and evaluate_unary_operation ctx unop expr =
  let expr = evaluate_expression ctx expr in
  match unop with
    | NameResolve.Not -> evaluate_not expr
    | NameResolve.UnaryMinus -> evaluate_unary_minus expr
    | NameResolve.UnaryPlus -> expr

and evaluate_vector_reduction ctx ranges expr =
  let rec evaluate_vector_reduction' ctx = function
    | [] -> evaluate_expression ctx expr
    | ranges -> vector_of_reduction ctx ranges
  and vector_of_reduction ctx = function
    | Range (Integer start, Integer step, Integer stop) :: ranges ->
        vector_of_range ctx start step stop ranges
    | ranges ->
        let ctx' =
          { ctx with
            class_context = ForContext (ctx, None)
          } in
        VectorReduction (ranges, evaluate_expression ctx' expr)
  and vector_of_range ctx start step stop ranges =
    let rec expression_list pred start = match pred start with
      | true -> []
      | false ->
          let ctx' =
            { ctx with
              class_context = ForContext (ctx, Some (Integer start))
            } in
          let expr = evaluate_vector_reduction' ctx' ranges in
          expr :: expression_list pred (Int32.add start step) in
    let cmp = Int32.compare step 0l in
    match cmp with
      | 0 when Int32.compare start stop <> 0 -> assert false (*error*)
      | 0 -> Vector [||]
      | _ when cmp < 0 ->
          let pred = function i -> Int32.compare i stop < 0 in
          let exprs = expression_list pred start in
          Vector (Array.of_list exprs)
      | _ ->
          let pred = function i -> Int32.compare i stop > 0 in
          let exprs = expression_list pred start in
          Vector (Array.of_list exprs) in
  let ranges = List.map (evaluate_expression ctx) ranges in
  evaluate_vector_reduction' ctx ranges

and evaluate_vector ctx exprs =
  let exprs = List.map (evaluate_expression ctx) exprs in
  Vector (Array.of_list exprs)

and evaluate_and expr expr' = match expr, expr' with
  | False, (False | True) | True, False -> False
  | True, True -> True
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_and exprs exprs')
  | _ -> BinaryOperation (And, expr, expr')

and evaluate_divide expr expr' = match expr, expr' with
  | _, Integer 0l -> assert false (*error*)
  | Integer 0l, _ -> Integer 0l
  | Integer i, Integer i' -> Integer (Int32.div i i')
  | Integer i, Real f -> Real (Int32.to_float i /. f)
  | Real f, Integer i -> Real (f /. Int32.to_float i)
  | Real f, Real f' -> Real (f /. f')
  | Vector exprs, _ ->
      let divide_element expr = evaluate_divide expr expr' in
      Vector (Array.map divide_element exprs)
  | _ -> BinaryOperation (Divide, expr, expr')

and evaluate_equalequal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i = i' -> True
  | Integer i, Real f | Real f, Integer i when f = Int32.to_float i -> True
  | Real f, Real f' when f = f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | Vector exprs, Vector exprs'
    when
      ArrayExt.for_all2
        (fun expr expr' -> evaluate_equalequal expr expr' = True)
        exprs
        exprs' -> True
  | Vector _, Vector _ -> False
  | _ -> BinaryOperation (EqualEqual, expr, expr')

and evaluate_greater_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i >= i' -> True
  | Integer i, Real f when Int32.to_float i >= f -> True
  | Real f, Integer i when f >= Int32.to_float i -> True
  | Real f, Real f' when f >= f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (GreaterEqual, expr, expr')

and evaluate_greater expr expr' = match expr, expr' with
  | Integer i, Integer i' when i > i' -> True
  | Integer i, Real f when Int32.to_float i > f -> True
  | Real f, Integer i when f > Int32.to_float i -> True
  | Real f, Real f' when f > f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (Greater, expr, expr')

and evaluate_less_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i <= i' -> True
  | Integer i, Real f when Int32.to_float i <= f -> True
  | Real f, Integer i when f <= Int32.to_float i -> True
  | Real f, Real f' when f <= f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (LessEqual, expr, expr')

and evaluate_less expr expr' = match expr, expr' with
  | Integer i, Integer i' when i < i' -> True
  | Integer i, Real f when Int32.to_float i < f -> True
  | Real f, Integer i when f < Int32.to_float i -> True
  | Real f, Real f' when f < f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | _ -> BinaryOperation (Less, expr, expr')

and evaluate_times expr expr' = match expr, expr' with
  | Integer 0l, _ | _, Integer 0l -> Integer 0l
  | Integer 1l, _ -> expr'
  | _, Integer 1l -> expr
  | Integer i, Integer i' -> Integer (Int32.mul i i')
  | Integer i, Real f | Real f, Integer i -> Real (f *. Int32.to_float i)
  | Real f, Real f' -> Real (f *. f')
  | Vector exprs, Vector exprs' -> assert false
  | _, Vector exprs | Vector exprs, _ -> assert false
  | _ -> BinaryOperation (Times, expr, expr')

and evaluate_not_equal expr expr' = match expr, expr' with
  | Integer i, Integer i' when i <> i' -> True
  | Integer i, Real f | Real f, Integer i when f <> Int32.to_float i -> True
  | Real f, Real f' when f <> f' -> True
  | (Integer _ | Real _), (Integer _ | Real _) -> False
  | Vector exprs, Vector exprs'
    when
      ArrayExt.exists2
        (fun expr expr' -> evaluate_equalequal expr expr' = False)
        exprs
        exprs' -> True
  | Vector _, Vector _ -> False
  | _ -> BinaryOperation (NotEqual, expr, expr')

and evaluate_or expr expr' = match expr, expr' with
  | True, (False | True) | False, True -> True
  | False, False -> False
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_or exprs exprs')
  | _ -> BinaryOperation (Or, expr, expr')

and evaluate_plus expr expr' = match expr, expr' with
  | Integer 0l, _ -> expr'
  | _, Integer 0l -> expr
  | Integer i, Integer i' -> Integer (Int32.add i i')
  | Integer i, Real f | Real f, Integer i -> Real (f +. Int32.to_float i)
  | Real f, Real f' -> Real (f +. f')
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_plus exprs exprs')
  | _ -> BinaryOperation (Plus, expr, expr')

and evaluate_power ctx expr expr' = match expr, expr' with
  | Integer 0l, Integer 0l ->
      raise (InstantError
        { err_msg = ["_ZeroRaisedToTheZeroPower"];
          err_info = [];
          err_ctx = ctx }) (*error*)
  | Integer 0l, (Integer _ | Real _) ->
      (* We know the answer for sure since second argument is constant *)
      Integer 0l
  | Integer i, Integer i' ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented";
                     "_IntegerRaisedToIntegerPower"];
          err_info = [];
          err_ctx = ctx })
  | Integer i, Real f -> Real (Int32.to_float i ** f)
  | Real f, Integer i ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented";
                     "_RealRaisedToIntegerPower"];
          err_info = [];
          err_ctx = ctx })
  | Real f, Real f' -> Real (f ** f')
  | Vector exprs, Integer i ->
      raise (InstantError
        { err_msg = ["_NotYetImplemented";
                     "_VectorRaisedToIntegerPower"];
          err_info = [];
          err_ctx = ctx })
  | _ -> BinaryOperation (Power, expr, expr')

and evaluate_minus expr expr' = match expr, expr' with
  | Integer 0l, _ -> evaluate_unary_minus expr'
  | _, Integer 0l -> expr
  | Integer i, Integer i' -> Integer (Int32.sub i i')
  | Integer i, Real f -> Real (Int32.to_float i -. f)
  | Real f, Integer i -> Real (f -. Int32.to_float i)
  | Real f, Real f' -> Real (f -. f')
  | Vector exprs, Vector exprs' ->
      Vector (ArrayExt.map2 evaluate_minus exprs exprs')
  | _ -> BinaryOperation (Minus, expr, expr')

and evaluate_class_function_invocation cl_def exprs =
  FunctionCall (ClassReference cl_def, exprs)

and evaluate_predefined_function_invocation ctx s exprs = match s with
  | "size" -> evaluate_size exprs
  | "reinit" | "der" | "pre" | "edge" | "change" | "initial" | "terminal" |
    "sample" | "delay" | "assert" | "terminate" | "abs" | "sign" | "cos" |
    "sin" | "tan" | "exp" | "log" | "sqrt" | "asin" | "acos" | "atan" |
    "sinh" | "cosh" | "tanh" | "asinh" | "acosh" | "atanh" | "log10" |
    "max" | "min" | "div" | "mod" | "rem" | "ceil" | "floor" | "integer" |
    "smooth" ->
      FunctionCall (PredefinedIdentifier s, exprs)
  | _ ->
      raise (InstantError
        { err_msg = ["_UnknownFunction"; s];
          err_info = [];
          err_ctx = ctx}) (*error*)

and evaluate_size exprs =
  let rec evaluate_size' expr i = match expr, i with
    | ComponentReference cpnt_desc, _ ->
        evaluate_component_size cpnt_desc i
    | Vector exprs, 1 -> Integer (Int32.of_int (Array.length exprs))
    | Vector exprs, _ -> evaluate_size' exprs.(0) (i - 1)
    | _ -> assert false (*error*)
  and evaluate_component_size cpnt_desc i =
    match evaluate cpnt_desc.component_nature, i with
      | DynamicArray _, _ -> FunctionCall (PredefinedIdentifier "size", exprs)
      | StaticArray cpnt_descs, 1 ->
          Integer (Int32.of_int (Array.length cpnt_descs))
      | StaticArray cpnt_descs, _ ->
          evaluate_component_size cpnt_descs.(i) (i - 1)
      | _ -> assert false (*error*)
  and evaluate_size_list = function
    | ComponentReference cpnt_desc -> assert false
    | Vector exprs ->
        let size = Integer (Int32.of_int (Array.length exprs)) in
        size :: evaluate_size_list exprs.(0)
    | _ -> [] in
  match exprs with
    | [expr] -> Vector (Array.of_list (evaluate_size_list expr))
    | [expr; Integer i] -> evaluate_size' expr (Int32.to_int i)
    | [expr; _] -> FunctionCall (PredefinedIdentifier "size", exprs)
    | _ -> assert false (*error*)

and evaluate_not expr = match expr with
  | True -> False
  | False -> True
  | Vector exprs -> Vector (Array.map evaluate_not exprs)
  | _ -> UnaryOperation (Not, expr)

and evaluate_unary_minus expr = match expr with
  | Integer i -> Integer (Int32.neg i)
  | Real f -> Real (~-. f)
  | Vector exprs -> Vector (Array.map evaluate_unary_minus exprs)
  | _ -> UnaryOperation (UnaryMinus, expr)

and field_access ctx expr id =
  let rec field_access' = function
    | ClassReference cl_def ->
        let cpnt_desc = create_temporary_instance ctx cl_def in
        component_field_access cpnt_desc
    | ComponentReference cpnt_desc -> component_field_access cpnt_desc
    | Record fields -> List.assoc id fields
    | Vector exprs -> Vector (Array.map field_access' exprs)
    | _ -> FieldAccess (expr, id)
  and component_field_access cpnt_desc =
    match evaluate cpnt_desc.component_nature with
      | DynamicArray _ -> FieldAccess (expr, id)
      | Instance inst -> instance_field_access ctx inst id
      | PredefinedTypeInstance _ -> assert false (*error*)
      | StaticArray cpnt_descs ->
          Vector (Array.map component_field_access cpnt_descs) in
  field_access' expr

and instance_field_access ctx inst id =
  let evaluate_component cpnt_desc =
    let evaluate_declaration_equation = function
      | Some expr -> evaluate expr
      | None ->
          raise (InstantError
            { err_msg = ["_MissingDeclEquForFixedId"; id];
              err_info = [];
              err_ctx = ctx}) (*error*) in
    let evaluate_parameter cpnt_nat =
      let evaluate_predefined_type_instance predef =
        match evaluate (List.assoc "fixed" predef.attributes) with
        | True -> evaluate_declaration_equation cpnt_desc.declaration_equation
        | False -> ComponentReference cpnt_desc
        | _ -> assert false (*error*) in
      match evaluate cpnt_nat with
      | PredefinedTypeInstance predef
        when List.mem_assoc "fixed" predef.attributes ->
          evaluate_predefined_type_instance predef
      | _ -> evaluate_declaration_equation cpnt_desc.declaration_equation in
    match cpnt_desc.variability with
      | Types.Constant ->
          evaluate_declaration_equation cpnt_desc.declaration_equation
      | Types.Parameter -> evaluate_parameter (cpnt_desc.component_nature)
      | _ -> ComponentReference cpnt_desc in
  let elts = evaluate inst.elements in
  let elt_desc = List.assoc id elts.named_elements in
  match evaluate elt_desc.element_nature with
    | Class cl_def -> ClassReference cl_def
    | Component cpnt_desc -> evaluate_component cpnt_desc

and expression_location ctx expr =
  match expr.NameResolve.info.NameResolve.syntax with
    | None -> ctx.location
    | Some expr -> expr.Syntax.info

and class_name_of_component cpnt_desc =
  let type_spec = Lazy.force cpnt_desc.NameResolve.type_specifier in
  let expr_info = type_spec.NameResolve.info in
  match expr_info.NameResolve.syntax with
      | None -> ""
      | Some expr -> Syntax.string_of_expression expr

and instance_nature_of_element elt_desc =
  match elt_desc.NameResolve.element_nature with
    | NameResolve.Component cpnt_desc ->
        ComponentElement (class_name_of_component cpnt_desc)
    | _ -> ClassElement

and instance_class_name instance_nature =
  match instance_nature with
    | ComponentElement s -> s
    | ClassElement -> ""

