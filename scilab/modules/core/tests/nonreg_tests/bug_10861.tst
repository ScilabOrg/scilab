// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10861 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10861
//
// <-- Short Description -->
//
// debug([]) returns a wrong error message

msgerr = msprintf(gettext("%s: Wrong size for argument #%d: Real scalar expected.\n"), "debug", 1);
assert_checkerror("debug([])", msgerr);
