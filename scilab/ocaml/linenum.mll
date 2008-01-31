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

{

let linenum = ref 0
let linebeg = ref 0

}

rule skip_line = parse
  | [^ '\n' '\r']* ('\n' | '\r' | "\r\n")
      { incr linenum;
        linebeg := Lexing.lexeme_start lexbuf;
        Lexing.lexeme_end lexbuf }
  | [^ '\n' '\r']* eof
      { incr linenum;
        linebeg := Lexing.lexeme_start lexbuf;
        raise End_of_file }

{

let for_position file loc =
  let ic = open_in_bin file in
  let lb = Lexing.from_channel ic in
  linenum := 1;
  linebeg := 0;
  begin try
    while skip_line lb <= loc do () done
  with End_of_file -> ()
  end;
  close_in ic;
  (!linenum - 1, !linebeg)

}
