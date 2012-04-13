// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->

loadXcosLibs();

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/fromws_tows_c.xcos"));

V_in = struct('time', (0:0.5:5000)', 'values', rand(10001, 1));
scicos_simulate(scs_m, list());

assert_checkequal(V_in.time, V_out.time);
assert_checkequal(V_in.values, V_out.values);

