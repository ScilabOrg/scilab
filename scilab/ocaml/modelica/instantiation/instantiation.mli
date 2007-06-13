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


(**)

val evaluate_toplevel_definitions: (string * element_description) list ->
  (string * NameResolve.element_description) list ->
  (string * element_description) list
