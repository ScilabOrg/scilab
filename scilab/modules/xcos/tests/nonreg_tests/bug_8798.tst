// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 -INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8798 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8798
//
// <-- Short Description -->
// The context parameter of scicos_simulate is not taken into account

assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/nonreg_tests/batch_simulation.zcos"));

scicos_simulate(scs_m, list(), "nw");
c1 = counter;
assert_checkequal(size(counter.time, "*"), 15);

Context.Pulsation = 6;
scicos_simulate(scs_m,list(), Context, "nw");
assert_checkequal(size(counter.time, "*"), 29);
