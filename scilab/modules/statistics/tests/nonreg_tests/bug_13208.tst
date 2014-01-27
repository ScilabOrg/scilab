// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13208 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13208
//
// <-- Short Description -->
// reglin returned NaNs when y (vector case) contained NaNs.
// =============================================================================

[a, b] = reglin(1:3, [1 %nan 5]);

assert_checkequal([a b], [2 -1]);
