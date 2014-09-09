// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
fptr_cos = funptr("cos");
newfun("cosAlias",fptr_cos);

assert_checkequal(cos(2*%pi), cosAlias(2*%pi));
assert_checktrue(clearfun("cosAlias"));
if execstr("cosAlias(2*%pi)","errcatch") ==  0  then pause,end

errmsg =  msprintf(gettext("%s: Wrong value for input argument #%d: Valid function name expected.\n"), "newfun", 1);
assert_checkerror('newfun(''1_function_name'',fptr_cos)', errmsg);

errmsg =  msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"), "newfun", 2);
assert_checkerror('newfun(''new_function_name'',[fptr_cos,fptr_cos])', errmsg);

errmsg =  msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"), "newfun", 1);
assert_checkerror('newfun([''new_function_name'',''new_function_name''],[fptr_cos,fptr_cos])', errmsg);

errmsg =  msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "newfun", 1);
assert_checkerror('newfun(1,[fptr_cos,fptr_cos])', errmsg);

function test_1(x)
    newfun(x, "cos");
    assert_checkequal(eval(x + "(1)"), cos(1));
endfunction

test_1("cosAlias");
assert_checkequal(cosAlias(1), cos(1));
assert_checktrue(clearfun("cosAlias"));

function test_2(x)
    test_1(x);
    assert_checkequal(eval(x + "(1)"), cos(1));
endfunction

test_2("cosAlias");
assert_checkequal(cosAlias(1), cos(1));
assert_checktrue(clearfun("cosAlias"));

function test_3(x)
    test_2(x);
    assert_checkequal(eval(x + "(1)"), cos(1));
endfunction

test_3("cosAlias");
assert_checkequal(cosAlias(1), cos(1));
assert_checktrue(clearfun("cosAlias"));
