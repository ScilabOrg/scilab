// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14107 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14107
//
// <-- Short Description -->
// lstcat of a string and a list does not produce consistent results.
assert_checkequal(lstcat("a", list(1)), list("a", 1));
