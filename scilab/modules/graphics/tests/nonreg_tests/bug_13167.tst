// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13167 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13167
//
// <-- Short Description -->
// fplot2d is fused into plot2d because it was too close to it.

// Check that the following tests match the examples on the plot2d help page (black curves),
// and for test #3, that the Y-axis is logarithmized

deff("y = f(x)", "y = sin(x)");
x=[0:0.1:2*%pi]';
// Overloading plot2d with a function
plot2d(x, f);
clf();
// Overloading plot2d with a string
plot2d(1:10, "parab");
clf();
plot2d("nl", 1:10, "parab"); // Test #3
clf();
plot2d(x, f, rect=[0,0,6,0.5]);
clf();
plot2d(x, f, [1,2,3], leg="L1@L2@L3", nax=[2,10,2,10], rect=[0,-2,2*%pi,2]);
clf();
plot2d(x, f, 1, frameflag=4);
clf();
plot2d(x, f, 1, frameflag=6);
clf();
plot2d(x, f, -1);
clf();
plot2d(x, f, 12);
clf();
plot2d(2*x, f, 3);
clf();
plot2d(x, f, leg="sin(x)");
a = gca(); // Handle on axes entity
a.y_location = "right";
