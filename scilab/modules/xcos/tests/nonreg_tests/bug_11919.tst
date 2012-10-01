// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 11919 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11919
//
// <-- Short Description -->
// dsslti4 crash Scilab when used without input or output

loadXcosLibs();
loadScicos();

importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_11919.zcos");

xcos_simulate(scs_m, 4);

