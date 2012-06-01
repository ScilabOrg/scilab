// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

pyAutounwrap(%f)

a = pyWrap(1:10);
pyAddVar("python_a",a);
b = pyGetVar("python_a");
assert_checkequal(pyUnwrap(b), [1,2,3,4,5,6,7,8,9,10]);
pyEvalStr("python_a.reverse()");

assert_checkequal(pyUnwrap(b),[10,9,8,7,6,5,4,3,2,1]);
c = pyGetVar("python_a");
assert_checkequal(pyUnwrap(c),[10,9,8,7,6,5,4,3,2,1]);

pyRemove a b c

a=["aze","zeazea","azeaze"]
pyAddVar("string_in_py",a)
b = pyGetVar("string_in_py");
assert_checkequal(pyUnwrap(b),a);

W=sprand(100,1000,0.001);
assert_checkerror("pyAddVar(''sparse_in_py'',W);", [], 999);

assert_checkerror("pyAddVar()", [], 77);
assert_checkerror("pyAddVar(2)", [], 77);
assert_checkerror("pyAddVar(2,2)", [], 999);
assert_checkerror("pyAddVar(2,2,2)", [], 77);

assert_checkerror("pyGetVar()", [], 77);
assert_checkerror("pyGetVar(3)", [], 999);
assert_checkerror("pyGetVar(3,3)", [], 77);


pyRemove a b
