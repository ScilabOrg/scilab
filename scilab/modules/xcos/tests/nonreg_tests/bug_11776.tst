// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 11776 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11776
//
// <-- Short Description -->
// CMSCOPE did not take into account label&Id parameter

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_11776.xcos"));

// compile and simulate
xcos_simulate(scs_m, 4);

// get figure properties of cscope and cmscope
figure_cmscope = scf(6789);
figure_cscope = scf(6790);

assert_checkequal(figure_cscope.figure_name, "CSCOPE");
assert_checkequal(figure_cmscope.figure_name, "CMSCOPE");

