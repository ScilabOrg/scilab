// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 8235 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8235
//
// <-- Short Description -->
// Masked superblock has their parameters overriden by the context.

importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_8235.xcos");
xcos_simulate(scs_m, 4);

if or(3 * A.values <> B.values) then pause, end

