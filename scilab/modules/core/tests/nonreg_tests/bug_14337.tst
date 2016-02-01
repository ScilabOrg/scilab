// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14337 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14337
//
// <-- Short Description -->
// piping one command using a shell pipepine ( "|" ) works, but scilab segfaults at exit

ierr = host("echo ""disp(%pi)""|bin/scilab-cli -ns");
assert_checkequal(ierr, 0);
