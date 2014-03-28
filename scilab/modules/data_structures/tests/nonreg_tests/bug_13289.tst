// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13289 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13289
//
// <-- Short Description -->
// Scilab crashed when inserting in a mlist

l = mlist(["A" "a"],1);
msg = gettext("Invalid index.");

assert_checkerror("a(rand(2,1))=0", msg);
