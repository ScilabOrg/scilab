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

(** Exceptions raised by typing functions *)
exception Type_error

(** the type of the types of the Modelica elements *)
type class_type =
  {
    partial: bool;
    kind: kind Lazy.t;
    named_elements: (string * element_type Lazy.t) list
  }

and kind =
  | Class
  | Model
  | Block
  | Record
  | ExpandableConnector
  | Connector
  | Package
  | Function

and element_type =
  {
    protected: bool;
    final: bool;
    replaceable: bool;
    dynamic_scope: dynamic_scope option;
    element_nature: element_nature
  }

and dynamic_scope =
  | Inner
  | Outer
  | InnerOuter

and element_nature =
  | ComponentElement of component_type
  | ClassElement of class_specifier Lazy.t
  | ComponentTypeElement of component_type
  | PredefinedTypeElement of predefined_type

and component_type =
  {
    flow: bool Lazy.t;
    variability: variability Lazy.t;
    causality: causality Lazy.t;
    base_class: class_specifier Lazy.t;
  }

and variability = Continuous | Discrete | Parameter | Constant

and causality = Acausal | Input | Output

and class_specifier =
  | PredefinedType of predefined_type
  | ClassType of class_type
  | ComponentType of component_type
  | ArrayType of dimension * class_specifier
  | TupleType of class_specifier list

and predefined_type =
  {
    base_type: base_type;
    attributes: (string * bool) list
 }

and base_type =
  | BooleanType
  | IntegerType
  | RealType
  | StringType
  | EnumerationType of string list

and dimension =
  | ConstantDimension of int32
  | ParameterDimension
  | DiscreteDimension


(** the result of the comparisons of two element types *)
type type_comparison =
  | NotRelated
  | Subtype
  | Supertype
  | SameType
  | CoercingType


(**)

(* Useful definitions *)

val boolean_class_type: class_specifier

val integer_class_type: class_specifier

val real_class_type: class_specifier

val string_class_type: class_specifier

val enumeration_class_type: string list -> class_specifier

val boolean_component_type: variability -> component_type

val integer_component_type: variability -> component_type

val real_component_type: variability -> component_type

val string_component_type: variability -> component_type

val enumeration_component_type: variability -> string list -> component_type

val empty_tuple_type: variability -> element_nature

val boolean_type: variability -> element_nature

val integer_type: variability -> element_nature

val integer_array_type: variability -> dimension -> element_nature

val real_type: variability -> element_nature

val string_type: variability -> element_nature

val enumeration_type: variability -> string list -> element_nature

val function_type: (string * component_type) list ->
  (string * component_type) list -> element_nature

val reversed_element_dimensions: element_nature -> dimension list


(* General type comparison functions *)

val compare_class_types: class_type -> class_type -> type_comparison
(** [compare_class_types] *)

val compare_element_natures: element_nature -> element_nature -> type_comparison
(** [compare_element_natures] *)

val compare_component_types: component_type -> component_type -> type_comparison
(** [compare_component_types] *)

val compare_predefined_types: predefined_type -> predefined_type -> type_comparison
(** [compare_predefined_types] *)

val compare_specifiers: class_specifier -> class_specifier -> type_comparison

(* type calculations *)

val min_variability: variability -> variability -> variability

val max_variability: variability -> variability -> variability

val higher_variability: variability -> variability -> bool

val lower_variability: variability -> variability -> bool

(* Pretty printing *)

val fprint_class_type: out_channel -> string -> class_type -> unit

val fprint_component_type: out_channel -> int -> string -> component_type -> unit

val fprint_class_specifier_type: out_channel -> int -> class_specifier -> unit

val fprint_predefined_type: out_channel -> predefined_type -> unit

(* Conversion of elements to string, used for error information display *)

val string_of_component_type: component_type -> string

val string_of_predefined_type: predefined_type -> string

val string_of_base_type: base_type -> string

val string_of_class_specifier: class_specifier -> string

val string_of_variability: variability -> string

val string_of_causality: causality -> string

val string_of_kind: kind -> string

val string_of_element_nature: element_nature -> string
