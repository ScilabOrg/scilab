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

let hello () =
  Printf.printf
    "-----------------------------------------------\n\
     Translator from Modelica 2.x to flat Modelica\n\
     Copyright (C) 2007 Imagine\n\
     Build 2007-05-03.\n\
     -----------------------------------------------\n";
  flush stderr;
  flush stdout

let lib_files = ref []

let add_library_file s = lib_files := !lib_files @ [s]

let command = ref None

let define_command s =
  if !command <> None then failwith "More than one command issued."
  else command := Some s

let filename = ref None

let define_filename s =
  if !filename <> None then failwith "More than one target filename specified."
  else filename := Some s

let function_directory = ref None

let define_function_directory s =
  if !function_directory <> None then
    failwith "More than one function directory specified."
  else function_directory := Some s

let set_input s = failwith ("Invalid command line argument: " ^ s)

let parse_args () =
  Arg.parse
    [("-lib", Arg.String add_library_file,
      "<filename> Define <filename> to be a file containing a Modelica \
      library of classes.");
     ("-o", Arg.String define_filename,
      "<filename> Specify target filename (default is `<instance name>.mo`).");
     ("-fundir", Arg.String define_function_directory,
      "<dirname> Specify the name of the directory where external function \
      prototypes have to be generated (default is \
      `<current directory>/functions`).");
     ("-command", Arg.String define_command,
      "\"command\" Define the component to be created as a regular Modelica \
      component declaration.")]
    set_input
    (Printf.sprintf
       "usage: %s [-lib <filename>] [-o <filename>] [-fundir <dirname>] \
        -command \"command\""
       Sys.argv.(0))

let parse_library lib_syns lib_file =
  let ic = open_in_bin lib_file in
  try
    let token_fun = Lexer.token
    and lexbuf = Lexing.from_channel ic in
    let src = Parser.LibraryFile lib_file in
    let lib_syn = Parser.parse src token_fun lexbuf in
    close_in ic;
    [lib_syn] @ lib_syns
  with 
    exn ->
      close_in ic;
      ExceptHandler.handle exn

let compile_libraries lib_files =
  try
    let lib_syns = List.fold_left parse_library [] lib_files in
    let comp_defs' = NameResolve.resolve_toplevel [] lib_syns in
    let inst_defs' =
      Instantiation.evaluate_toplevel_definitions [] comp_defs' in
    comp_defs', inst_defs'
  with 
    exn ->
      ExceptHandler.handle exn

let create_component comp_defs inst_defs filename fun_dir cmd =
  try
    let token_fun = Lexer.token
    and lexbuf = Lexing.from_string cmd in
    let inst_syn = Parser.parse Parser.CommandLine token_fun lexbuf in
    let comp_defs' = NameResolve.resolve_toplevel comp_defs [inst_syn] in
    let inst_defs' =
      Instantiation.evaluate_toplevel_definitions inst_defs comp_defs' in
    CodeGeneration.generate_code filename fun_dir inst_defs'
  with 
    exn ->
      ExceptHandler.handle exn

let run () =
  hello();
  parse_args ();
  let comp_defs, inst_defs =
    compile_libraries !lib_files in
  let cmd = match !command with
    | None -> failwith "Command missing"
    | Some cmd -> cmd in
  create_component comp_defs inst_defs !filename !function_directory cmd

;;

run ()
