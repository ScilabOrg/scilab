// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13243 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13243
//
// <-- Short Description -->
// optim() with "gc" option failed when imp<0.

function f = problem1(x)
    f = sum((x-(1:length(x))').^2);
endfunction

// Define the cost function:
function [f, g, ind] = myoptfunc(x, ind)
    f = problem1(x);
    g = derivative(problem1, x, order=4);
endfunction

// Initial guess:
x0 = 1*ones(5, 1);

// Perform optimization:
[fopt, xopt] = optim(myoptfunc, x0, "gc", imp=-1);

assert_checkequal(fopt, 0);
assert_checkequal(xopt', 1:5);
