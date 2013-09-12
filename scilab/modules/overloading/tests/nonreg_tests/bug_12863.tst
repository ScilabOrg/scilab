// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12863 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12863
//
// <-- Short Description -->
// Function size(state-space, "r") does not work

sl = ssrand(3, 2, 4);
assert_checkequal(size(sl), [3, 2]);
assert_checkequal(size(sl, "r"), 3);
assert_checkequal(size(sl, "c"), 2);
