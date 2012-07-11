// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8296 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8296
//
// <-- Short Description -->
// size( M,3) crashed scilab 5.3.0-beta-4.

M = ones(3, 2);
assert_checkequal(size(M,1), 3);
assert_checkequal(size(M,2), 2);
assert_checkequal(size(M,"r"), 3);
assert_checkequal(size(M,"c"), 2);
assert_checkequal(size(M,"*"), 6);
assert_checkequal(size(M), [3, 2]);

assert_checkequal(size(M, 3), 1);
assert_checkequal(size(M, 5), 1);
assert_checktrue(execstr("size(M, 0)", "errcatch") <> 0);
assert_checktrue(execstr("size(M,""a"")", "errcatch") <> 0);

[n1, n2, n3, n4] = size(M);
assert_checktrue( (n1 == 3) & (n2 == 2) & (n3 == 1) & (n4 == 1));

M=ones(3, 2, 3);
assert_checkequal(size(M),[3, 2, 3]);
assert_checkequal(size(M, 5), 1);
assert_checkequal(size(M, 1), 3);
assert_checkequal(size(M, 2), 2);
assert_checkequal(size(M, 3), 3);
[n1, n2] = size(M);
assert_checktrue((n1 == 3) & (n2 == 2));
[n1, n2, n3] = size(M);
assert_checktrue( (n1 == 3) & (n2 == 2) & (n3 == 3));
[n1, n2, n3, n4] = size(M);
assert_checktrue((n1 == 3) & (n2 == 2) & (n3 == 3) & (n4 == 1));

A = ones(3,2);
msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"),"size",2);
assert_checkerror ( "size(A, 1.2)" , msgerr);
