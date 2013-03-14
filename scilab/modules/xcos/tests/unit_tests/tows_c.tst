// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/xcos/tests/unit_tests/tows_c.tst");
//  or test_run('xcos', 'tows_c', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/tows_c.xcos"));

// Reference values
t = (2.2:0.1:14.9)';
A_ref = struct('values', sin(t), 'time', t);
B_ref = [1+2*%i 3+4*%i; 1 2];
E_ref = int32(ones(128, 2));
E_ref(:, 2) = 2*E_ref(:, 2);

// Run simulation + save results
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref));
assert_checkalmostequal(A.values, A_ref.values);
assert_checkalmostequal(A.time, A_ref.time);
assert_checkequal(fieldnames(C), fieldnames(A_ref));
assert_checkalmostequal(C.values, A_ref.values);
assert_checkequal(fieldnames(D), fieldnames(A_ref));
assert_checkalmostequal(D.values, A_ref.values);

assert_checkequal(B.values(:,:,128), B_ref);

assert_checktrue(E.values == E_ref);
