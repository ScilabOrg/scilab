// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10816 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10816
//
// <-- Short Description -->
// Typo fixed in %ip_part error message.

refMsg = msprintf(_("%s: Wrong value for input argument #%d: $ expected.\n"), "part", 2);

s = poly(0, "s");
assert_checkerror("part(""string"", s:s:s)", refMsg);
