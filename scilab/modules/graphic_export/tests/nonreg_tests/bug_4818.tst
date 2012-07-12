// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 4818 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4818
//
// <-- Short Description -->
// It was not possible to export a figure with a lot of circle marks.

x = 1:1000;
y = log(x);
plot(x, y, '.');
xs2svg(0, TMPDIR + "/bug_4818.svg");