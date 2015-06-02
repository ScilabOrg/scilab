// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12202 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12202
//
// <-- Short Description -->
// int8(50):2:152 has a different behavior between Scilab 5 & 6

assert_checkequal(int8(150):2:150, int8(150));

assert_checkequal(int8(50):50:123, int8([50 100]));