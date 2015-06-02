// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// // <-- INTERACTIVE TEST -->
// <-- Non-regression test for bug 13725 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13725
//
// <-- Short Description -->
// Polyline is not correctly filled with big values

y = [0, -4, 2, -3, -1, -1, 7, 0]
x = [0, 1, 3, 6, 16, 7, 3, 0]
scf();
xfpoly(x*1e18, y, 3)

// check that the polyline is correctly filled
