// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 10836 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10836
//
// <-- Short Description -->
// resize_matrix with boolean failed.

myMat = 5 * rand(3, 4);
myMatBoolean = resize_matrix( myMat, 2, 2, "boolean");
ref = (zeros(2,2) == 1);
assert_checkequal(myMatBoolean, ref);

myMat(1, 1) = 1;
myMat(3, 4) = 1;
ref = (zeros(2, 2) == 1);
ref(1, 1) = %t;
myMatBoolean = resize_matrix( myMat, 2, 2, "boolean");
assert_checkequal(myMatBoolean, ref);

myMat(1, 1) = 1;
myMat(3, 4) = 1;
ref = (zeros(10, 10) == 1);
ref(1, 1) = %t;
ref(3, 4) = %t;
myMatBoolean = resize_matrix( myMat, 10, 10, "boolean");
assert_checkequal(myMatBoolean, ref);

myMat = ["Scilab" "5"; ..
"Test"   "bug_10836"];
myMatBoolean = resize_matrix( myMat, 2, 2, "boolean");
ref = (zeros(2,2) == 1);
assert_checkequal(myMatBoolean, ref);

myMat = 5 * rand(3, 4);
msgerr = msprintf(gettext("%s: Wrong value for input argument #%d.\n"), "resize_matrix", 1);
assert_checkerror("myMatBoolean = resize_matrix( myMat, 2, 2, 999)", msgerr);

myMat = ones(3, 4);
ref = string(ones(2, 2));
myNewMat = resize_matrix( myMat, 2, 2, "string");
assert_checkequal(myNewMat, ref);