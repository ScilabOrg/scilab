(* ceci est un premier commentaire *)
(*  Scicos *)
(* *)
(*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr> *)
(* *)
(* This program is free software; you can redistribute it and/or modify *)
(* it under the terms of the GNU General Public License as published by *)
(* the Free Software Foundation; either version 2 of the License, or *)
(* (at your option) any later version. *)
(* *)
(* This program is distributed in the hope that it will be useful, *)
(* but WITHOUT ANY WARRANTY; without even the implied warranty of *)
(* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *)
(* GNU General Public License for more details. *)
(* *) 
(* You should have received a copy of the GNU General Public License *)
(* along with this program; if not, write to the Free Software *)
(* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. *)
(*  *)
(* See the file ./license.txt *)

(** This module provides functions that generate code for the Scicos target. *)

val generate_code : string -> string -> string -> Optimization.model -> bool
  -> unit
(** [generate_code path filename fun_name model jac] generates the code that
allows the numeric simulation of [model]. [path] is the path to the external
functions referenced in [model]. [filename] is the name of the file where the
code is generated. [fun_name] is the name of the entry point in the generated
code. [jac] is a flag which true value indicates that a symbolic jacobian is
requested. *)
