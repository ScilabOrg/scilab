// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 10269 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10269
//
// <-- Short Description -->
//    qp_solve can now take up to 5 output arguments. The last one is an error flag,
//    if it is present, then the function will issue a warning instead of an error.

// Run with test_run('optimization', 'bug_10269', ['no_check_error_output'])

C1 = [1, -1, 2; -1, 0, 5; 1, -3, 3; 0, -4, 0; 3, 5, 1; 1, 6, 0];
b1 = [1; 2; 3];

// x'*C2 >= b2 (2 inequality constraints)
C2 = [0 ,1; -1, 0; 0, -2; -1, -1; -2, -1; 1, 0];
b2 = [1; -2.5];

// and minimize 0.5*x'*Q*x - p'*x with
p = [-1; -2; -3; -4; -5; -6]; Q = eye(6, 6);
me = 3;

// Normal behavior
[x, iact, iter, f] = qp_solve(Q, p, [C1 C2], [b1; b2], me);
assert_checkequal(iter, [5; 0]);
assert_checkalmostequal(f, -14.843248, 1e-5);

// Still normal, but with error flag
[x, iact, iter, f, info] = qp_solve(Q, p, [C1 C2], [b1; b2], me);
assert_checkequal(info, 0);
assert_checkequal(iter, [5; 0]);
assert_checkalmostequal(f, -14.843248, 1e-5);

// Provoked error, without flag
Q = rand(6, 6);
refMsg = msprintf(_("%s: Q is not symmetric positive definite.\n"), "qp_solve");
assert_checkerror("[x, iact, iter, f] = qp_solve(Q, p, [C1 C2], [b1; b2], me);", refMsg);

// Provoked error, with flag
[x, iact, iter, f, info] = qp_solve(Q, p, [C1 C2], [b1; b2], me);
assert_checkequal(info, 2);


