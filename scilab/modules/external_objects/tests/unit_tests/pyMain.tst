// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

main = pyMain();
main.a = 123;
pyEvalStr("a = a + 321");
r = main.a;
assert_checkequal(main.a,444);
assert_checkequal(r,444);
assert_checkerror("pyMain(32)",[],999);
//pyRemove main r
