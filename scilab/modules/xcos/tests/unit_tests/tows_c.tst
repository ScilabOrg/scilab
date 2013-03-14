// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Execute with test_run('xcos', 'tows_c', ['no_check_error_output']);

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/tows_c.xcos"));

loadScicos();
loadXcosLibs();
// Reference values
t = (2.2:0.1:14.9)';
A_ref = struct('values', sin(t), 'time', t); // A_ref = (sin(t), t)
B_ref = [1+2*%i 3+4*%i; 1 2]; // B_ref is an element of hypermatrix B
E_ref = int32(ones(128, 2));  // E_ref = [1, 1] with 128 rows
E_ref(:, 2) = 2*E_ref(:, 2);  // E_ref = [1, 2] with 128 rows

// Run simulation with scicos_simulate() + check results
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values
assert_checkequal(fieldnames(D), fieldnames(A_ref)); // Check D fields
assert_checkalmostequal(D.values, A_ref.values);     // Check D values

assert_checkequal(B.values(:,:,128), B_ref);         // Check that B.values is a hypermatrix with correct values

assert_checktrue(E.values == E_ref);                 // Check that E contains an int32 vector

// Run simulation with xcos_simulate() + check results
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values
assert_checkequal(fieldnames(D), fieldnames(A_ref)); // Check D fields
assert_checkalmostequal(D.values, A_ref.values);     // Check D values

assert_checkequal(B.values(:,:,128), B_ref);         // Check that B.values is a hypermatrix with correct values

assert_checktrue(E.values == E_ref);                 // Check that E contains an int32 vector

// Type checks
assert_checkequal(type(A.values), 1);         // A takes real numbers
assert_checkequal(type(B.values(:,:,1)), 1); // B takes complex numbers
assert_checkequal(type(E.values), 8);        // E takes 4-octets coded integers

// Permanent variable protection
ierr = 0;
scs_m.objs(3).graphics.exprs(2)='%i'; // Giving '%i' as variable to block A
ierr = execstr("scicos_simulate(scs_m, ''nw'');", 'errcatch'); // Trying to run the simulation
assert_checkequal(ierr, 13); // Check that "Redefining permanent variable." error (number 13) has popped

ierr = 0;
scs_m.objs(3).graphics.exprs(2)='%e'; // Same, with '%e'
ierr = execstr("scicos_simulate(scs_m, ''nw'');", 'errcatch');
assert_checkequal(ierr, 13);
