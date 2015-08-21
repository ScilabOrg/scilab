// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13709 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13709
//
// <-- Short Description -->
//    Calling grand(m,n) returned a wrong error and grand(m,n,p) called %s_grand
//    function instead of an error.
// =============================================================================

assert_checkfalse(execstr("grand(2,3)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "grand", 3);
assert_checkerror("grand(2,3)", refMsg);

assert_checkfalse(execstr("grand(4,3,2)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "grand", 4);
assert_checkerror("grand(4,3,2)", refMsg);
