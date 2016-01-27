type instantiated_class = {
  icl_components : (string * instantiated_component Lazy.t) list Lazy.t;
  icl_init_equs : equation list Lazy.t;
  icl_equs : equation list Lazy.t;
}
and instantiated_component =
    InstantiatedParameter of instantiated_parameter
  | InstantiatedVariable of instantiated_variable
and instantiated_parameter =
    InstantiatedIntegerParameter of string * parameter_kind *
      typed_expression * Compilation.variable_infos
  | InstantiatedStringParameter of string * parameter_kind *
      typed_expression * Compilation.variable_infos
  | InstantiatedRealParameter of string * parameter_kind * typed_expression *
      Compilation.variable_infos
and parameter_kind = Main | Sub
and instantiated_variable =
    InstantiatedIntegerVariable of string * Compilation.inout *
      typed_expression * Compilation.variable_infos
  | InstantiatedStringVariable of string * Compilation.inout *
      typed_expression * Compilation.variable_infos
  | InstantiatedDiscreteVariable of string * Compilation.inout *
      typed_expression * Compilation.variable_infos
  | InstantiatedRealVariable of string * Compilation.inout *
      Compilation.nature * typed_expression * Compilation.variable_infos
  | InstantiatedCompoundVariable of string * typed_expression
and equation =
    Equation of typed_expression * typed_expression
  | ConditionalEquation of (typed_expression * equation list) list *
      equation list
  | FlowConnection of typed_expression * typed_expression
  | When of when_clause_type
and when_clause_type = (typed_expression * typed_when_expression list) list
and typed_when_expression =
    Reinit of typed_expression * typed_expression
  | Assign of typed_expression * typed_expression
and typed_expression = {
  tex_type : expression_type;
  tex_expression : expression option;
}
and expression_type =
    BooleanType of int array
  | CartesianProduct of expression_type list
  | CompoundType of int array
  | IntegerType of int array
  | RealType of int array
  | StringType of int array
and expression =
    Abs of typed_expression
  | Acos of typed_expression
  | Acosh of typed_expression
  | Addition of typed_expression * typed_expression
  | And of typed_expression * typed_expression
  | Asin of typed_expression
  | Asinh of typed_expression
  | Atan of typed_expression
  | Atanh of typed_expression
  | Boolean of bool
  | Cardinality of typed_expression
  | CompoundElement of instantiated_class
  | Cos of typed_expression
  | Cosh of typed_expression
  | Der of typed_expression
  | Division of typed_expression * typed_expression
  | Equals of typed_expression * typed_expression
  | Exp of typed_expression
  | ExternalFunctionCall of string list * expression_type list *
      expression_type list * typed_argument list
  | Floor of typed_expression
  | GreaterEqualThan of typed_expression * typed_expression
  | GreaterThan of typed_expression * typed_expression
  | If of (typed_expression * typed_expression) list * typed_expression
  | Integer of int32
  | Log of typed_expression
  | Max of typed_expression * typed_expression
  | Min of typed_expression * typed_expression
  | Mod of typed_expression * typed_expression
  | Minus of typed_expression
  | Multiplication of typed_expression * typed_expression
  | NoEvent of typed_expression
  | Not of typed_expression
  | NotEquals of typed_expression * typed_expression
  | Or of typed_expression * typed_expression
  | ParameterValue of int * reference
  | Power of typed_expression * typed_expression
  | Pre of typed_expression
  | Real of float
  | Sin of typed_expression
  | Sinh of typed_expression
  | Sqrt of typed_expression
  | String of string
  | Subtraction of typed_expression * typed_expression
  | Tan of typed_expression
  | Tanh of typed_expression
  | Time
  | VariableStart of int * reference
  | VariableValue of int * reference
  | Vector of typed_expression array
and typed_argument =
    ScalarArgument of typed_expression
  | ArrayArgument of int list * typed_expression array
and reference = (string * int array) list
and instantiation_context =
    ToplevelContext
  | ClassContext of instantiation_context * instantiated_class Lazy.t
  | ForContext of instantiation_context * int
and modification =
    Modification of (string * int array) * modification list *
      typed_expression option
val string_of_expression : typed_expression -> string
val separate_parameters_from_variables :
  Compilation.compiled_component list ->
  Compilation.parameter list * Compilation.variable list
val separate_inputs_from_others :
  Compilation.variable list ->
  Compilation.variable list * Compilation.variable list
val separate_outputs_from_others :
  Compilation.variable list ->
  Compilation.variable list * Compilation.variable list
val instantiate_main_class :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.compiled_class -> typed_expression
val instantiate_main_components :
  instantiation_context ->
  Compilation.compiled_modification list ->
  (string * Compilation.compiled_component Lazy.t) list Lazy.t ->
  (string * instantiated_component Lazy.t) list
val instantiate_main_parameter :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.parameter -> instantiated_parameter
val instantiate_class :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.compiled_class -> typed_expression
val instantiate_components :
  instantiation_context ->
  Compilation.compiled_modification list ->
  (string * Compilation.compiled_component Lazy.t) list Lazy.t ->
  (string * instantiated_component Lazy.t) list
val filter_modification_of :
  string ->
  Compilation.compiled_modification list ->
  Compilation.compiled_modification -> Compilation.compiled_modification list
val instantiate_parameter :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.parameter -> instantiated_parameter
val initialize_parameter :
  instantiation_context ->
  typed_expression ->
  Compilation.compiled_modification list ->
  Compilation.parameter_attributes ->
  (string ->
   typed_expression -> Compilation.variable_infos -> instantiated_parameter) ->
  instantiated_parameter
val calculate_initial_value :
  instantiation_context ->
  int array ->
  typed_expression ->
  Compilation.compiled_modification list ->
  Compilation.compiled_expression option -> typed_expression
val instantiate_variable :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.variable -> instantiated_variable
val initialize_compound_variable :
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.compiled_class Lazy.t ->
  Compilation.variable_attributes -> instantiated_variable
val init_array :
  int array ->
  instantiation_context ->
  Compilation.compiled_modification list ->
  Compilation.compiled_modification list ->
  Compilation.compiled_class -> typed_expression
val merge_modifications :
  Compilation.compiled_modification list ->
  Compilation.compiled_modification list ->
  Compilation.compiled_modification list
val initialize_base_type_variable :
  instantiation_context ->
  typed_expression ->
  Compilation.compiled_modification list ->
  Compilation.variable_attributes ->
  (string ->
   Compilation.inout ->
   Compilation.nature ->
   typed_expression -> Compilation.variable_infos -> instantiated_variable) ->
  instantiated_variable
val create_array : int array -> typed_expression -> typed_expression
val instantiate_equations :
  instantiation_context ->
  Compilation.compiled_equation list Lazy.t -> equation list
val instantiate_equation :
  instantiation_context ->
  equation list -> Compilation.compiled_equation -> equation list
val evaluate_integer_expression :
  instantiation_context -> Compilation.compiled_expression -> expression
val instantiate_when_equations :
  instantiation_context ->
  Compilation.compiled_when_expression list -> typed_when_expression list
val instantiate_argument :
  instantiation_context -> Compilation.compiled_argument -> typed_argument
val instantiate_expression :
  instantiation_context ->
  Compilation.compiled_expression -> typed_expression
val check_function_type :
  instantiation_context ->
  Compilation.compiled_class Lazy.t ->
  typed_argument list -> expression_type list * expression_type list
val instantiate_if_alternatives :
  instantiation_context ->
  expression_type ->
  (Compilation.compiled_expression * Compilation.compiled_expression) list ->
  expression_type * (typed_expression * typed_expression) list
val compute_array_type : typed_expression array -> expression_type
val expand_for_equation :
  instantiation_context ->
  int -> int -> int -> Compilation.compiled_equation list -> equation list
val get_component_reference :
  instantiation_context ->
  Compilation.level -> Compilation.path -> typed_expression
val get_loop_variable_value :
  instantiation_context -> Compilation.level -> expression
val find_component_level :
  instantiation_context -> Compilation.level -> instantiated_class
val find_instantiated_component :
  instantiation_context ->
  instantiated_class ->
  Compilation.level -> Compilation.path -> typed_expression
val search_into_parameter :
  instantiation_context ->
  string ->
  Compilation.compiled_subscript array ->
  Compilation.level ->
  Compilation.field list -> instantiated_parameter -> typed_expression
val search_into_variable :
  instantiation_context ->
  string ->
  Compilation.compiled_subscript array ->
  Compilation.level ->
  Compilation.field list -> instantiated_variable -> typed_expression
val search_into_compound_variable :
  instantiation_context ->
  string ->
  int array ->
  Compilation.level ->
  Compilation.path -> typed_expression -> typed_expression
val compute_subscript :
  instantiation_context -> Compilation.compiled_subscript -> int
val find_subvector_dims : int array -> typed_expression -> int array
val get_compound_subvector :
  int array -> typed_expression -> typed_expression
val no_out_of_bounds_access : int array -> int array -> bool
val map2 : ('a -> 'b -> 'c) -> 'a array -> 'b array -> 'c array
val array_map :
  (typed_expression -> expression) -> typed_expression -> typed_expression
val array_map2 :
  (typed_expression -> typed_expression -> expression) ->
  typed_expression -> typed_expression -> typed_expression
val list_of_array : typed_expression -> typed_expression list
val expand_class :
  typed_expression ->
  (string * instantiated_component) list * equation list * equation list
val flatten_instantiated_class :
  string ->
  instantiated_class ->
  (string * instantiated_component) list * equation list * equation list
val collect_flows :
  (string * instantiated_component) list -> typed_expression list list
val flatten_components :
  string ->
  (string * instantiated_component Lazy.t) list Lazy.t ->
  (string * instantiated_component) list * equation list * equation list
val flatten_component_tree :
  (typed_expression -> instantiated_component) ->
  string ->
  string -> typed_expression -> (string * instantiated_component) list
val explode_compound_components :
  (string * instantiated_component) list ->
  (string * instantiated_component) list * equation list * equation list
val update_equations : string -> equation list -> equation list
val update_equation : string -> equation -> equation
val update_if_clauses :
  string ->
  (typed_expression * equation list) list ->
  (typed_expression * equation list) list
val update_when_clauses : string -> when_clause_type -> when_clause_type
val update_typed_expression : string -> typed_expression -> typed_expression
val update_expression : string -> expression option -> expression option
val update_expression' : string -> expression -> expression
val update_typed_argument : string -> typed_argument -> typed_argument
val update_reference : int -> string -> reference -> reference
val update_path : int -> string -> string
val flatten_equations : equation list -> equation list
val flatten_equation : equation -> equation list
val flatten_if_clauses :
  (typed_expression * equation list) list ->
  (typed_expression * equation list) list
val flatten_when_clauses : when_clause_type -> when_clause_type
val flatten_typed_expression : typed_expression -> typed_expression
val flatten_typed_expression' :
  typed_expression -> expression -> typed_expression
val flatten_typed_argument : typed_argument -> typed_argument
val flatten_multiplication :
  typed_expression ->
  typed_expression -> typed_expression -> typed_expression
val expand_identifier :
  expression_type ->
  (reference -> expression) -> int -> reference -> typed_expression
val perform_connections :
  typed_expression list list -> equation list -> equation list
val collect_connected_components : equation list -> string list
val evaluate_cardinalities : string list -> equation list -> equation list
