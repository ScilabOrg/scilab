// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13146 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13146
//
// <-- Short Description -->
// profile failed when a comment was at the same line as a function declaration.

function A() // Comment
endfunction

add_profiling("A");
A();
prof = profile(A);

assert_checkequal(prof(:, 2:3), [0 1 ; 0 0]);
