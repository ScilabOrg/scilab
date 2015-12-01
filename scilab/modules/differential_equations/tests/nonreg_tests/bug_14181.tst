// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14181 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=14181
//
// <-- Short Description -->
// Calling intg (or integrate) in a function that is being integrated failed

function y = f(x)
    y = x.*integrate("x^2", "x", 0, 1)
endfunction
function z = g(x)
    z = x.*integrate("f(x)", "x", 0, 1)
endfunction

x = 1:10;
b = g(x);
bRef = (0.5:0.5:5)/3;

assert_checkalmostequal(b, bRef);
