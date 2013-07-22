// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/logic.zcos"));

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checktrue(A.values(1:8)   == 0);
assert_checktrue(A.values(9:18)  == 1);
assert_checktrue(A.values(19:28) == 0);
assert_checktrue(A.values(29:38) == 1);
