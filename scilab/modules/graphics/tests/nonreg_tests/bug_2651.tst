// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2651 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2651
//
// <-- Short Description -->
// plot2d was not working when the x parameter was not given and some optional argument was given.

x = [1:.1:2];
y = sin(x);

plot2d(y);
plot2d(x,y);

plot2d(y, style=3);
plot2d(x, y, style=3);

plot2d("ll", y);
plot2d("ll", x, y);
plot2d("ll", y, style=3);
plot2d("ll", x, y, style=3);

plot2d([0;1],[0;1],0,"121","plot",[0, 0, 1, 1],[1,10,1,10]);
