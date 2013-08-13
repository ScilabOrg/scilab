// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/switch2_m.zcos"));


// In the diagram, SWITCH2_m block is given int32, double and int32 as input types (3, 1 and 3).
// This test ensures that a block can have heterogeneous input types.
Five  = 5*ones(29, 1);
A_ref = int32([-Five; Five; 5; 5; -Five; -5; -5; -5; 5; 5; 5; 5; 5]);

try scicos_simulate(scs_m); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);


// Now, try with int16 inputs. select_mode = 1 does not produce any change in this case.
scs_m.props.context = ["a=int16(-5)"; "b=int16(5)"; "input_type=4"; "select_mode=1"; "buffsize=97"];
A_ref = int16(A_ref);

try scicos_simulate(scs_m); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);


// Finally, with int8 inputs, and select_mode = 2, we set buffsize to 100 to catch the first zero of the sine function.
// After that first zero, the sine will not be exactly zero at the testing points anymore.
scs_m.props.context = ["a=int8(-5)"; "b=int8(5)"; "input_type=5"; "select_mode=2"; "buffsize=100"];
A_ref = int8([5; -5*ones(99, 1)]);

try scicos_simulate(scs_m); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end
assert_checkequal(A.values, A_ref);
