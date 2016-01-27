exception Infinite_result of string
type t = {
  nature : nature;
  sortingHash : int;
  hash : int;
  mutable count : int;
  mutable replacement : t;
}
and nature =
    Addition of t list
  | And of t list
  | ArcCosine of t
  | ArcHyperbolicCosine of t
  | ArcHyperbolicSine of t
  | ArcHyperbolicTangent of t
  | ArcSine of t
  | ArcTangent of t
  | BlackBox of string * argument list
  | BooleanValue of bool
  | Constant of string
  | Cosine of t
  | Derivative of t * Num.num
  | DiscreteVariable of int
  | Equality of t * t
  | Exponential of t
  | Floor of t
  | Greater of t * t
  | GreaterEqual of t * t
  | HyperbolicCosine of t
  | HyperbolicSine of t
  | HyperbolicTangent of t
  | If of t * t * t
  | Integer of int32
  | Logarithm of t
  | Multiplication of t list
  | Not of t
  | Number of Num.num
  | Or of t list
  | Parameter of int
  | PartialDerivative of t * t
  | Pre of t
  | RationalPower of t * Num.num
  | Sign of t
  | Sine of t
  | String of string
  | Tangent of t
  | TimeVariable
  | Variable of int
and argument = ScalarArgument of t | ArrayArgument of int list * t array
val nature : t -> nature
val hash : t -> int
val argument_hash : argument -> int
val eq_array : 'a array -> 'a array -> bool
val exists_array : ('a -> bool) -> 'a array -> bool
val eq_list : 'a list -> 'a list -> bool
val insert : t -> t list -> t list
val remove : 'a -> 'a list -> 'a list
val sort : t list -> t list
val union : 'a list -> 'a list -> 'a list
val intersection : 'a list -> 'a list -> 'a list
type node = t
module Node : sig type t = node val hash : t -> int end
module NodeSet :
  sig
    type elt = Node.t
    type 'a t = 'a GraphNodeSet.Make(Node).t
    val create :
      int -> ('a -> elt -> bool) -> ('a -> int) -> ('a -> int -> elt) -> 'a t
    val find_or_add : 'a -> 'a t -> elt
    val iter : (elt -> unit) -> 'a t -> unit
  end
val global_count : int ref
val unique_integer : unit -> int
val create_node : nature -> int -> t
val additionNodeSet : NodeSet.elt list NodeSet.t
val andNodeSet : NodeSet.elt list NodeSet.t
val arcCosineNodeSet : NodeSet.elt NodeSet.t
val arcHyperbolicCosineNodeSet : NodeSet.elt NodeSet.t
val arcHyperbolicSineNodeSet : NodeSet.elt NodeSet.t
val arcHyperbolicTangentNodeSet : NodeSet.elt NodeSet.t
val arcSineNodeSet : NodeSet.elt NodeSet.t
val arcTangentNodeSet : NodeSet.elt NodeSet.t
val blackBoxNodeSet : (string * argument list) NodeSet.t
val constantNodeSet : string NodeSet.t
val cosineNodeSet : NodeSet.elt NodeSet.t
val derivativeNodeSet : (NodeSet.elt * Num.num) NodeSet.t
val discreteVariableNodeSet : int NodeSet.t
val equalityNodeSet : (NodeSet.elt * NodeSet.elt) NodeSet.t
val exponentialNodeSet : NodeSet.elt NodeSet.t
val floorNodeSet : NodeSet.elt NodeSet.t
val greaterNodeSet : (NodeSet.elt * NodeSet.elt) NodeSet.t
val greaterEqualNodeSet : (NodeSet.elt * NodeSet.elt) NodeSet.t
val hyperbolicCosineNodeSet : t NodeSet.t
val hyperbolicSineNodeSet : NodeSet.elt NodeSet.t
val hyperbolicTangentNodeSet : NodeSet.elt NodeSet.t
val ifNodeSet : (NodeSet.elt * NodeSet.elt * NodeSet.elt) NodeSet.t
val integerNodeSet : int32 NodeSet.t
val logarithmNodeSet : NodeSet.elt NodeSet.t
val multiplicationNodeSet : NodeSet.elt list NodeSet.t
val numberNodeSet : Num.num NodeSet.t
val notNodeSet : NodeSet.elt NodeSet.t
val orNodeSet : NodeSet.elt list NodeSet.t
val parameterNodeSet : int NodeSet.t
val partialDerivativeNodeSet : (NodeSet.elt * NodeSet.elt) NodeSet.t
val preNodeSet : NodeSet.elt NodeSet.t
val rationalPowerNodeSet : (NodeSet.elt * Num.num) NodeSet.t
val signNodeSet : NodeSet.elt NodeSet.t
val sineNodeSet : NodeSet.elt NodeSet.t
val stringNodeSet : string NodeSet.t
val tangentNodeSet : NodeSet.elt NodeSet.t
val variableNodeSet : int NodeSet.t
val zero_num : Num.num
val one_num : Num.num
val two_num : Num.num
val minus_one : NodeSet.elt
val zero : NodeSet.elt
val one : NodeSet.elt
val two : NodeSet.elt
val ten : NodeSet.elt
val one_over_two : NodeSet.elt
val minus_one_over_two : NodeSet.elt
val pi : NodeSet.elt
val pi_over_two : NodeSet.elt
val minus_pi_over_two : NodeSet.elt
val e : NodeSet.elt
val eps : NodeSet.elt
val false_value : t
val true_value : t
val time : t
val addition_neutral : NodeSet.elt
val multiplication_neutral : NodeSet.elt
val and_neutral : t
val or_neutral : t
val create_addition : NodeSet.elt list -> NodeSet.elt
val create_and : NodeSet.elt list -> NodeSet.elt
val create_arcCosine : NodeSet.elt -> NodeSet.elt
val create_arcHyperbolicCosine : NodeSet.elt -> NodeSet.elt
val create_arcHyperbolicSine : NodeSet.elt -> NodeSet.elt
val create_arcHyperbolicTangent : NodeSet.elt -> NodeSet.elt
val create_arcSine : NodeSet.elt -> NodeSet.elt
val create_arcTangent : NodeSet.elt -> NodeSet.elt
val create_blackBox : string -> argument list -> NodeSet.elt
val create_booleanValue : bool -> t
val create_constant : string -> NodeSet.elt
val create_cosine : NodeSet.elt -> NodeSet.elt
val create_derivative : NodeSet.elt -> Num.num -> NodeSet.elt
val create_discrete_variable : int -> NodeSet.elt
val create_equality : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val create_exponential : NodeSet.elt -> NodeSet.elt
val create_floor : NodeSet.elt -> NodeSet.elt
val create_greater : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val create_greater_equal : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val create_hyperbolicCosine : t -> NodeSet.elt
val create_hyperbolicSine : NodeSet.elt -> NodeSet.elt
val create_hyperbolicTangent : NodeSet.elt -> NodeSet.elt
val create_if : NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val create_integer : int32 -> NodeSet.elt
val create_logarithm : NodeSet.elt -> NodeSet.elt
val create_multiplication : NodeSet.elt list -> NodeSet.elt
val create_not : NodeSet.elt -> NodeSet.elt
val create_number : Num.num -> NodeSet.elt
val create_or : NodeSet.elt list -> NodeSet.elt
val create_parameter : int -> NodeSet.elt
val create_partialDerivative : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val create_pre : NodeSet.elt -> NodeSet.elt
val create_rationalPower : NodeSet.elt -> Num.num -> NodeSet.elt
val create_sign : NodeSet.elt -> NodeSet.elt
val create_sine : NodeSet.elt -> NodeSet.elt
val create_string : string -> NodeSet.elt
val create_tangent : NodeSet.elt -> NodeSet.elt
val create_timeVariable : unit -> t
val create_variable : int -> NodeSet.elt
val apply_if_possible :
  (NodeSet.elt list -> NodeSet.elt) ->
  (NodeSet.elt -> NodeSet.elt -> NodeSet.elt option) ->
  NodeSet.elt -> NodeSet.elt list -> NodeSet.elt option
val symbolic_abs : NodeSet.elt -> NodeSet.elt
val symbolic_acos : NodeSet.elt -> NodeSet.elt
val symbolic_acosh : NodeSet.elt -> NodeSet.elt
val symbolic_add : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_and : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_asin : NodeSet.elt -> NodeSet.elt
val symbolic_asinh : NodeSet.elt -> NodeSet.elt
val symbolic_atan : NodeSet.elt -> NodeSet.elt
val symbolic_atanh : NodeSet.elt -> NodeSet.elt
val symbolic_blackBox : string -> argument list -> NodeSet.elt
val symbolic_cos : NodeSet.elt -> NodeSet.elt
val symbolic_cosh : NodeSet.elt -> NodeSet.elt
val symbolic_derive : NodeSet.elt -> Num.num -> NodeSet.elt
val symbolic_derivative : NodeSet.elt -> NodeSet.elt
val symbolic_div : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_eq : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_exp : NodeSet.elt -> NodeSet.elt
val symbolic_ge : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_gt : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_if : NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_floor : NodeSet.elt -> NodeSet.elt
val symbolic_invert : NodeSet.elt -> NodeSet.elt
val symbolic_le : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_log : NodeSet.elt -> NodeSet.elt
val symbolic_log10 : NodeSet.elt -> NodeSet.elt
val symbolic_lt : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_max : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_min : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_minus : NodeSet.elt -> NodeSet.elt
val symbolic_mult : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_neq : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_not : NodeSet.elt -> NodeSet.elt
val symbolic_or : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_partial_derivative : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_partial_derivative_with :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_power : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_pre : NodeSet.elt -> NodeSet.elt
val symbolic_rationalPower : NodeSet.elt -> Num.num -> NodeSet.elt
val symbolic_sgn : NodeSet.elt -> NodeSet.elt
val symbolic_sin : NodeSet.elt -> NodeSet.elt
val symbolic_sinh : NodeSet.elt -> NodeSet.elt
val symbolic_sqrt : NodeSet.elt -> NodeSet.elt
val symbolic_sub : NodeSet.elt -> NodeSet.elt -> NodeSet.elt
val symbolic_tan : NodeSet.elt -> NodeSet.elt
val symbolic_tanh : NodeSet.elt -> NodeSet.elt
val symbolic_add_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val add_number_to_addition_if_possible :
  NodeSet.elt -> NodeSet.elt list -> NodeSet.elt option
val add_factors_if_possible :
  Num.num ->
  NodeSet.elt list -> Num.num -> NodeSet.elt list -> NodeSet.elt option
val add_multiplication_to_multiplication_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val add_expression_to_multiplication_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val symbolic_mult_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val mult_expression_by_power_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val mult_power_by_power_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val symbolic_and_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val symbolic_or_if_possible :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val apply_addition : NodeSet.elt list -> NodeSet.elt
val apply_and : NodeSet.elt list -> NodeSet.elt
val apply_blackBox : string -> argument list -> NodeSet.elt
val apply_max : NodeSet.elt list -> NodeSet.elt
val apply_min : NodeSet.elt list -> NodeSet.elt
val apply_multiplication : NodeSet.elt list -> NodeSet.elt
val apply_or : NodeSet.elt list -> NodeSet.elt
val output : out_channel -> NodeSet.elt -> unit
val exists : (NodeSet.elt -> bool) -> NodeSet.elt -> bool
val is_subnode_of : NodeSet.elt -> NodeSet.elt -> bool
val assignable_parameters_of : t -> t list
val variables_of : t -> t list
val assignable_variables_of : t -> t list
val derivatives_of : NodeSet.elt -> NodeSet.elt list
val inputs_of : t -> t list
val invert_if_possible_with_respect_to :
  NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> NodeSet.elt option
val exists_except_in_conditions : (t -> bool) -> t -> bool
val inversion_difficulty : NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> int
val replace : NodeSet.elt -> NodeSet.elt -> NodeSet.elt -> NodeSet.elt
