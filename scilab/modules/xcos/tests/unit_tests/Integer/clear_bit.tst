// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/clear_bit.zcos"));

A_ref = int16(36*ones(128,1));

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checkequal(A.values, A_ref);
