// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for isfield function
// =============================================================================

my_struct = struct("field_1",123,"field_2",456);

// syntax

msg = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isfield",2);
assert_checkerror("isfield()",msg);

assert_checkerror("isfield(my_struct)", msg);

msg = msprintf(gettext("Wrong number of input arguments."));
assert_checkerror("isfield(my_struct,''field_1'',''field_2'')", msg);

msg = msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"), "isfield", 2);
assert_checkerror("isfield(list(my_struct, my_struct),[''field_1'',''field_2''])", msg);

msg = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"isfield",2);
assert_checkerror("isfield(my_struct,2)",msg);

my_struct = ["field_1","field_2"];

msg = msprintf(gettext("%s: Wrong type for input argument #%d: struct array, tlist, mlist or list of struct array or tlist or mlist expected.\n"),"isfield",1);
assert_checkerror("isfield(my_struct, ''field_1'')", msg);

assert_checkerror("isfield(list(1,2,3), ''field_1'')", msg);


// Fonctionnality

my_struct = struct("field_1",123,"field_2",456,"field_4",789);

assert_checktrue(isfield(my_struct, "field_1"));
assert_checktrue(isfield(my_struct, "field_2"));
assert_checkfalse(isfield(my_struct, "field_3"));
assert_checktrue(isfield(my_struct, "field_4"));

fields = [ "field_1" "field_2" ; "field_3" "field_4" ; "field_3" "field_4" ];
ref    = [    %T        %T     ;    %F        %T     ;    %F        %T     ];
assert_checkequal(isfield(my_struct, fields), ref);


fields = [ "field_1" "field_2" "field_3" "field_4" "field_3" "field_4" ];
ref    = [    %T        %T        %F        %T        %F        %T     ];
assert_checkequal(isfield(my_struct, fields), ref);

fields = [ "field_1" ; "field_2" ; "field_3" ; "field_4" ; "field_3" ; "field_4" ];
ref    = [    %T     ;    %T     ;    %F     ;    %T     ;    %F     ;    %T     ];
assert_checkequal(isfield(my_struct, fields), ref);

fields = [ "field_1" "field_2" "field_3" ; "field_4" "field_3" "field_4" ];
ref    = [    %T        %T        %F     ;    %T        %F        %T     ];
assert_checkequal(isfield(my_struct, fields), ref);

s = tlist(["test" "t" "n","","p"], "Hello", %pi, "anonymous", (1-%z)^3);
a=isfield(s, ["b" "t" ""]);
assert_checkequal(a, [%f %t %t]);

// List of struct
p=struct();
p.f1=%z;
p2=p;
p2.a=%pi;
assert_checkequal(isfield(list(p,p2),"a"), [%f;%t]);

// List of tlist
s1 = tlist(["test" "a" "n","","p"], "Hello", %pi, "anonymous", (1-%z)^3);
assert_checkequal(isfield(list(s,s1),"a"), [%f;%t]);

// List of mlist
M = mlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);
M1 = mlist(['V','name1','value1'],['a','b';'c' 'd'],[1 2; 3 4]);
assert_checkequal(isfield(list(M,M1),"name"), [%t;%f]);

// Mix
assert_checkequal(isfield(list(s,M),"name"), [%f;%t]); //tlist and mlist
assert_checkequal(isfield(list(s,M),"t"), [%t;%f]); //tlist and mlist
assert_checkequal(isfield(list(p,M),"f1"), [%t;%f]); //struct and mlist
assert_checkequal(isfield(list(p,M),"name"), [%f;%t]); //struct and mlist
assert_checkequal(isfield(list(p,s),"t"), [%f;%t]); //struct and tlist
assert_checkequal(isfield(list(p,s),"f1"), [%t;%f]); //struct and tlist
