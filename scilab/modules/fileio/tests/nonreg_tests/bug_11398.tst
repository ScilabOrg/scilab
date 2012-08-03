// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11398 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11398
//
// <-- Short Description -->
// newest() produces an incomprehensible error message when called with a string vector as second input argument.
//

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "newest", 2);
assert_checkerror("newest(""SCI/modules/fileio/macros/lib"", ls(""SCI/modules/fileio/macros/*.*""))", refMsg);