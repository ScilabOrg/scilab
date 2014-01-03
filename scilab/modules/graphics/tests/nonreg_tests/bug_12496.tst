// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12496 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12496
//
// <-- Short Description -->
// zoom_rect could lead to a crash in log scale

x=[432462,17893,78004,10269];
y=[168.75,206.25,187.5,225];
plot2d(x,y,-2)
a = gca();
a.grid = [color("black"),color("black")];
a.log_flags = "ll";
a.sub_ticks = [8 0];
zoom_rect([10000,100,500000,250]);
