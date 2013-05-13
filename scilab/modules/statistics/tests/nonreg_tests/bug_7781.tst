// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7781 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7781
//
// <-- Short Description -->
//     The second parameter of iqr function had no effect.
// =============================================================================
X = [1:10; 2:2:20; 3:3:30];

assert_checkequal(iqr(X,'r'), [1.5 3 4.5 6 7.5 9 10.5 12 13.5 15]);
assert_checkequal(iqr(X,'c'), [5 10 15]');
