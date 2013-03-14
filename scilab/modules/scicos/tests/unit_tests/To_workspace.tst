// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/To_workspace.tst");
//  or test_run('scicos', 'To_workspace', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/To_workspace.xcos"));

// Run simulation + save results
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end // Run simulation
try Values = A.values; catch disp(lasterror()); end       // Save results
try Time = A.time; catch disp(lasterror()); end           // Save time
try Values = B.values; catch disp(lasterror()); end       // Save results
try Time = B.time; catch disp(lasterror()); end           // Save time
try Values = C.values; catch disp(lasterror()); end       // Save results
try Time = C.time; catch disp(lasterror()); end           // Save time
try Values = D.values; catch disp(lasterror()); end       // Save results
try Time = D.time; catch disp(lasterror()); end           // Save time
try Values = E.values; catch disp(lasterror()); end       // Save results
try Time = E.time; catch disp(lasterror()); end           // Save time

F = ones(128, 2);

assert_checktrue(D.values == E);
