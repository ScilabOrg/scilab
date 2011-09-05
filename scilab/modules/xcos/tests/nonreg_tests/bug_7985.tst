// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 7985 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7985
//
// <-- Short Description -->
// dmmul.c define an unoptimized dgemm version

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_7985.xcos"));

xcos_simulate(scs_m, 4);
assert_checktrue(isdef(A));
assert_checkequals();
