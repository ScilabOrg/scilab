// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 10276 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10276
//
// <-- Short Description -->
// qpsolve() crashed when used on a too large problem
//

stacksize("max");

m = 7;
n = 10000;
Q = eye(n, n);
p = zeros(n, 1);
data   = rand(m, n);
obj    = rand(m, 1);
menini = rand(n, 1);

refMsg = msprintf(_("%s: Cannot allocate more memory.\n"), "qp_solve");
assert_checkerror("qpsolve(Q, p, data, obj, -menini, 5*menini, m);", refMsg);
