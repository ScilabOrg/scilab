// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9935 -->
//
// <-- JVM NOT MANDATORY -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9935
//
// <-- Short Description -->
// clear() did not clear all variables

%helps = "toto";
clear()
assert_checkequal(isdef('%helps'), %f);

%helps = "toto";
clear('%helps');
assert_checkequal(isdef('%helps'), %f);
