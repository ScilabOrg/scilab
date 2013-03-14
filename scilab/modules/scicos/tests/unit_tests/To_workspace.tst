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
scs_m.props.tf = 13;
try xcos_simulate(scs_m, 4); catch disp(lasterror()); end // Run simulation
try Values = res.values; catch disp(lasterror()); end     // Save results
try Time = res.time; catch disp(lasterror()); end         // Save time
