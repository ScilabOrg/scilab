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

and expression_information =
  {
    syntax: Parser.location Syntax.expression option;
    type_description: Types.element_nature
  }

and class_definition =
  {
    class_type: Types.class_specifier Lazy.t;
    enclosing_class: class_definition option;
    encapsulated: bool;
    description: class_description Lazy.t;
  }

and class_description =
  | LongDescription of long_description
  | ShortDescription of modified_class

and long_description =
  {
    class_annotations: annotation list;
    imports: import_description list;
    extensions: (visibility * modified_class) list;
    named_elements: named_element list;
    unnamed_elements: equation_or_algorithm_clause list Lazy.t;
    external_call: external_call option Lazy.t
  }

and annotation = inverse_function Lazy.t

and inverse_function =
  {
    function_class: expression;
    arguments: (string * string) list
  }

and import_description = unit

and visibility = Public | Protected

and external_call = (external_call_desc, Parser.location Syntax.externalll) node

and external_call_desc =
  | PrimitiveCall of string
  | ExternalProcedureCall of language *
      expression option (* rhs *) * string * expression list

and language = C | FORTRAN

and modified_class =
  {
    modified_class_type: Types.class_type Lazy.t;
    base_class: expression Lazy.t;
    class_modification: class_modification Lazy.t
  }

and named_element = string * element_description

and element_description =
  {
    element_type: Types.element_type Lazy.t;
    redeclare: bool;
    element_nature: element_nature;
    element_location: Parser.location
  }

and element_nature =
  | Component of component_description
  | Class of class_definition
  | ComponentType of component_type_description
  | PredefinedType of Types.predefined_type

and component_type_description =
  {
    described_type: Types.component_type Lazy.t;
    base_type: expression Lazy.t;
    type_dimensions: dimension list Lazy.t;
    type_modification: class_modification Lazy.t
  }

and component_description =
  {
    component_type: Types.component_type Lazy.t;
    type_specifier: expression Lazy.t;
    dimensions: dimension list Lazy.t;
    modification: modification option Lazy.t;
    comment: string
  }

and dimension =
  | Colon
  | Expression of expression

and class_modification = modification_argument list

and modification_argument =
  {
    each: bool;
    final: bool;
    target: string;
    action: modification_action option
  }

and modification_action =
  | ElementModification of modification
  | ElementRedeclaration of element_description

and modification =
  | Modification of class_modification * expression Lazy.t option
  | Assignment of expression Lazy.t
  | Equality of expression Lazy.t

and equation_or_algorithm_clause =
  | EquationClause of validity * equation list
  | AlgorithmClause of validity * algorithm list

and validity = Initial | Permanent

and equation = (equation_desc, Parser.location Syntax.equation option) node

and equation_desc =
  | Equal of expression * expression
  | ConditionalEquationE of (expression * equation list) list * equation list
  | ForClauseE of expression list (* ranges *) * equation list
  | ConnectFlows of sign * expression * sign * expression
  | WhenClauseE of (expression * equation list) list

and sign = Positive | Negative

and algorithm = (algorithm_desc, Parser.location Syntax.algorithm option) node

and algorithm_desc =
  | Assign of expression * expression
  | FunctionCallA of expression * expression list
  | MultipleAssign of expression list * expression * expression list
  | Break
  | Return
  | ConditionalEquationA of (expression * algorithm list) list *
      algorithm list
  | ForClauseA of expression list (* ranges *) * algorithm list
  | WhileClause of expression * algorithm list
  | WhenClauseA of (expression * algorithm list) list

and expression = (expression_desc, expression_information) node

and expression_desc =
  | BinaryOperation of binary_operator_kind * expression * expression
  | DynamicIdentifier of int (* number of nested classes to skip *) *
      string (* name to be searched for at instanciation time *)
  | False
  | FieldAccess of expression * string
  | FunctionArgument of int (* the position of the argument in the call *)
  | FunctionCall of expression (* function *) *
      expression list (* arguments *) *
      expression (* the expression involving the function call *)
      (* creation of a dynamic function context *)
  | FunctionInvocation of expression list
      (* invocation of the current function in context *)
  | If of (expression (* condition *) * expression) list *
      expression (* default *)
  | IndexedAccess of expression * expression list (* subscripts *)
  | Integer of int32
  | LocalIdentifier of int (* number of nested classes to skip *) *
      string (* key in the dictionary of the defining class *)
  | LoopVariable of int (* number of nested for loops to skip *)
  | NoEvent of expression
  | PredefinedIdentifier of string (* predefined identifier *)
  | Range of expression * expression * expression
  | Real of float
  | String of string
  | ToplevelIdentifier of string (* key in the toplevel dictionary *)
  | True
  | Tuple of expression list
  | UnaryOperation of unary_operator_kind * expression
  | Vector of expression list
  | VectorReduction of expression list (* nested ranges *) * expression

and unary_operator_kind =
  | Not
  | UnaryMinus
  | UnaryPlus

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


(* Context types. Contexts are used to resolve identifiers in expressions *)

type context =
  {
    toplevel: (string * element_description) list Lazy.t;
    context_nature: context_nature;
    location: Parser.location
  }

and context_nature =
  | ToplevelContext
  | ClassContext of class_definition
  | SubscriptContext of
      context * expression (* evaluating to an array *) *
      int32 (* dimension index *) * Types.dimension
  | ForContext of context * string * Types.element_nature

(** Error description *)
type error_description =
  {
    err_msg: string list;
    err_info: (string * string) list;
    err_ctx: context
  }

(** Error detected during compilation *)
exception CompilError of error_description

(**)

val resolve_toplevel:
  (string * element_description) list ->
  (Parser.location Syntax.toplevel_element_desc, Parser.location) Syntax.node list ->
  (string * element_description) list

