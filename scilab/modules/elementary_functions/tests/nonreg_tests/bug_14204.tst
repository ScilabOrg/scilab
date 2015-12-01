// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14204 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14204
//
// <-- Short Description -->
// dec2bin ( dec2base ) must show a better error message for too large values.

err = msprintf(_("%s: Wrong value for input argument #%d: Must be between 0 and 2^52.\n"), "dec2base", 1);
assert_checkerror("dec2bin(uint64(2^53))", err, 999);
