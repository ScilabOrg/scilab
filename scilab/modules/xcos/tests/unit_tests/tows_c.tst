// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/tows_c.xcos"));

// Reference values
t = (2.2 : 0.1 : 14.9)';
A_ref = struct('values', sin(t), 'time', t); // A_ref = (sin(t), t)
B_ref = [1+2*%i 3+4*%i ; 1 2]; // B_ref is an element of hypermatrix B
E_ref = int32([1 2 ; 3 4]);     // E_ref is an element of hypermatrix E
intV_ref = int32(ones(128, 2));
intV_ref = 5*intV_ref;
intV_ref(:, 2) = intV_ref(:, 2) + 1;     // intV_ref = [5, 6] with 128 rows
shortV_ref = int16(ones(128, 2));
shortV_ref = 7*shortV_ref;
shortV_ref(:, 2) = shortV_ref(:, 2) + 1; // shortV_ref = [7, 8] with 128 rows
charV_ref = int8(ones(128, 2));
charV_ref = 9*charV_ref;
charV_ref(:, 2) = charV_ref(:, 2) + 1;   // charV_ref = [9, 10] with 128 rows
intM_ref = int32([11 12 ; 13 14]);  // intM_ref is an element of hypermatrix intM
shortM_ref = int16([15 16; 17 18]); // shortM_ref is an element of hypermatrix shortM
charM_ref = int8([19 20 ; 21 22]);  // charM_ref is an element of hypermatrix charM

// Run simulation with scicos_simulate() + check results
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values
assert_checkequal(fieldnames(D), fieldnames(A_ref)); // Check D fields
assert_checkalmostequal(D.values, A_ref.values);     // Check D values

assert_checktrue(intV.values   == intV_ref);    // Check that intV contains an int32 vector
assert_checktrue(shortV.values == shortV_ref);  // Check that shortV contains an int16 vector
assert_checktrue(charV.values  == charV_ref);   // Check that charV contains an int8 vector
for i = 1:128
    // Check that the following are hypermatrices with correct values
    assert_checktrue(and(B.values(:, :, i)      == B_ref));
    assert_checktrue(and(E.values(:, :, i)      == E_ref));
    assert_checktrue(and(intM.values(:, :, i)   == intM_ref));
    assert_checktrue(and(shortM.values(:, :, i) == shortM_ref));
    assert_checktrue(and(charM.values(:, :, i)  == charM_ref));
end

// Run simulation with xcos_simulate() + check results
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end  // Run simulation
assert_checkequal(fieldnames(A), fieldnames(A_ref)); // Check A fields
assert_checkalmostequal(A.values, A_ref.values);     // Check A values
assert_checkalmostequal(A.time, A_ref.time);         // Check A time values
assert_checkequal(fieldnames(C), fieldnames(A_ref)); // Check C fields
assert_checkalmostequal(C.values, A_ref.values);     // Check C values
assert_checkequal(fieldnames(D), fieldnames(A_ref)); // Check D fields
assert_checkalmostequal(D.values, A_ref.values);     // Check D values

assert_checktrue(intV.values   == intV_ref);    // Check that intV contains an int32 vector
assert_checktrue(shortV.values == shortV_ref);  // Check that shortV contains an int16 vector
assert_checktrue(charV.values  == charV_ref);   // Check that charV contains an int8 vector
for i = 1:128
    // Check that the following are hypermatrices with correct values
    assert_checktrue(and(B.values(:, :, i)      == B_ref));
    assert_checktrue(and(E.values(:, :, i)      == E_ref));
    assert_checktrue(and(intM.values(:, :, i)   == intM_ref));
    assert_checktrue(and(shortM.values(:, :, i) == shortM_ref));
    assert_checktrue(and(charM.values(:, :, i)  == charM_ref));
end

// Type checks
assert_checkequal(type(A.values), 1);            // A takes real numbers
assert_checkequal(type(B.values(:, :, 1)), 1);  // B takes complex numbers
assert_checkequal(type(E.values(1)), 8);
assert_checkequal(type(intV.values(1)), 8);
assert_checkequal(type(shortV.values(1)), 8);
assert_checkequal(type(charV.values(1)), 8);
assert_checkequal(type(intM.values(1)), 8);
assert_checkequal(type(shortM.values(1)), 8);
assert_checkequal(type(charM.values(1)), 8);
assert_checkequal(inttype(E.values(1)), 4);      // E takes 4-octets coded integers
assert_checkequal(inttype(intV.values(1)), 4);   // intV takes 4-octets coded integers
assert_checkequal(inttype(shortV.values(1)), 2); // shortV takes 2-octets coded integers
assert_checkequal(inttype(charV.values(1)), 1);  // charV takes 1-octets coded integers
assert_checkequal(inttype(intM.values(1)), 4);
assert_checkequal(inttype(shortM.values(1)), 2);
assert_checkequal(inttype(charM.values(1)), 1);

// Permanent variable protection
ierr = 0;
scs_m.objs(3).graphics.exprs(2) = '%i'; // Giving '%i' as variable to block A
ierr = execstr("scicos_simulate(scs_m, ''nw'');", 'errcatch'); // Trying to run the simulation
assert_checkequal(ierr, 13); // Check that "Redefining permanent variable." error (number 13) has popped

ierr = 0;
scs_m.objs(3).graphics.exprs(2) = '%e'; // Same, with '%e'
ierr = execstr("scicos_simulate(scs_m, ''nw'');", 'errcatch');
assert_checkequal(ierr, 13);
