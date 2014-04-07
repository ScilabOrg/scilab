// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13168 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13168
//
// <-- Short Description -->
// Invalid interpretation of star followed by minus

assert_checkequal(1*2*-2 + 1, -3);
assert_checkequal(2*2*2*-2 + 1, -15);
assert_checkequal(2*2*-2*2 + 1, -15);