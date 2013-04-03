// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10415 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10415
//
// <-- Short Description -->
// The removal of an history session line does not remove
// all the children command lines

resethistory();

// a session line will be added at the front of this history
addhistory("lineA");
addhistory("lineB");
addhistory("lineC");

// we check the removal of current session line remove all history
removelinehistory(0);
h = gethistory();
assert_checkequal(h, []);
