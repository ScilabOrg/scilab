// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 14040 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14040
//
// <-- Short Description -->
// graphic property setting fails when using array of handles

clf;
xstring(0.1,0.1,"foo1");e=gce();H(1,1)=e;
xstring(0.1,0.2,"foo2");e=gce();H(2,1)=e;

H(1,1).text="text1";
assert_checkequal(H(1).text, "text1");
H(2,1).text="text2";
assert_checkequal(H(2).text, "text2");
H(1:2).text="text3";
assert_checkequal(H(1).text, "text3");
assert_checkequal(H(2).text, "text3");
