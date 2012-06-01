// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
main = pyMain();
main.a = 1234;
myFun="def foo(): ...
    return 43";
pyEvalStr(myFun);
assert_checkerror("pyEvalStr(32)",[],999);

ret = pyEvalStr("print a * 3", %t);
assert_checkequal(ret,"3702.0");
assert_checkequal(main.foo(),int32(43));
