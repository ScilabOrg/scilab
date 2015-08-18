// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13843 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13843
//
// <-- Short Description -->
// plot2d and polarplot crashes Scilab with wrong strf argument

// plot2d
f = scf();
x = linspace(0,10,100);
errmsg = msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "plot2d", 4);
assert_checkerror("plot2d(x, sin(x),1, 111);", errmsg);

// polarplot
clf();
t = 0:.01:2*%pi; 
assert_checkerror("polarplot(sin(7*t),cos(8*t), [2 3], 111);", errmsg);
