type token =
  | STRING of (string)
  | EOF

open Parsing;;
# 23 "stringParser.mly"


# 10 "stringParser.ml"
let yytransl_const = [|
    0 (* EOF *);
    0|]

let yytransl_block = [|
  257 (* STRING *);
    0|]

let yylhs = "\255\255\
\001\000\001\000\001\000\000\000"

let yylen = "\002\000\
\000\000\001\000\002\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\002\000\000\000\003\000"

let yydgoto = "\002\000\
\004\000"

let yysindex = "\255\255\
\000\255\000\000\000\000\001\255\000\000"

let yyrindex = "\000\000\
\003\000\000\000\000\000\004\000\000\000"

let yygindex = "\000\000\
\000\000"

let yytablesize = 4
let yytable = "\001\000\
\003\000\005\000\001\000\004\000"

let yycheck = "\001\000\
\001\001\001\001\000\000\000\000"

let yynames_const = "\
  EOF\000\
  "

let yynames_block = "\
  STRING\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun parser_env ->
    Obj.repr(
# 38 "stringParser.mly"
                            ( "" )
# 61 "stringParser.ml"
               : string))
; (fun parser_env ->
    let _1 = (peek_val parser_env 0 : string) in
    Obj.repr(
# 39 "stringParser.mly"
                            ( _1 )
# 68 "stringParser.ml"
               : string))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : string) in
    let _2 = (peek_val parser_env 0 : string) in
    Obj.repr(
# 40 "stringParser.mly"
                            ( _1 ^ _2 )
# 76 "stringParser.ml"
               : string))
(* Entry parse_string *)
; (fun parser_env -> raise (YYexit (peek_val parser_env 0)))
|]
let yytables =
  { actions=yyact;
    transl_const=yytransl_const;
    transl_block=yytransl_block;
    lhs=yylhs;
    len=yylen;
    defred=yydefred;
    dgoto=yydgoto;
    sindex=yysindex;
    rindex=yyrindex;
    gindex=yygindex;
    tablesize=yytablesize;
    table=yytable;
    check=yycheck;
    error_function=parse_error;
    names_const=yynames_const;
    names_block=yynames_block }
let parse_string (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (yyparse yytables 1 lexfun lexbuf : string)
;;
# 43 "stringParser.mly"


let parse token_fun lexbuf =
  try
    parse_string token_fun lexbuf
  with
    | _ -> assert false
# 109 "stringParser.ml"
