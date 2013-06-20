// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12034 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12034
//
// <-- Short Description -->
// The max function does not manage empty matrix.

assert_checkequal(max([],[],[]), []);
errmsg = msprintf(_("null matrix (argument # %d).\n"), 1);
assert_checkerror("max([],1,1)", errmsg, 45);
errmsg2 = msprintf(_("null matrix (argument # %d).\n"), 2);
assert_checkerror("max(1,[],[])", errmsg2, 45);
errmsg3 = msprintf(_("null matrix (argument # %d).\n"), 3);
assert_checkerror("max(1,1,[])", errmsg3, 45);
