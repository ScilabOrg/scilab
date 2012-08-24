// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11689 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11689
//
// <-- Short Description -->
// The window was resized after showing.

f=gdf();
f.position = [500 500 300 100];
f.background = 2;
f.children.background = 3;
gcf()

// The window must be displayed directly with the good size.
