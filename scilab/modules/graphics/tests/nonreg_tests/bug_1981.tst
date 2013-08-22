// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1981 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1981
//
// <-- Short Description -->
// Print or export of an image that is created by Matplot does not
// resemble the image shown on the screen.

// check that the line is drawn plain
clf();xset("thickness",1);plot2d(sin(5*(0:0.01:2*%pi)));

