// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/LSodar.tst");
//  or test_run('scicos', 'LSodar', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/LSodar_test.zcos"));

// Set solver to LSodar + run LSodar + save results
scs_m.props.tol(6) = 0;											// Set solver to LSodar
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end;	// Run LSodar
lsodarval = res.values;		// Results
time = res.time;			// Time

// Set solver to CVode BDF/Newton + run + save results
scs_m.props.tol(6) = 1;
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end;
cvval = res.values;

// Compare results
compa = abs(lsodarval-cvval);

// Extract mean, standard deviation, maximum
mea = mean(compa);
[maxi, indexMaxi] = max(compa);
stdeviation = st_deviation(compa);

// Verifying closeness of the results
assert_checktrue(maxi <= 10^-8);
assert_checktrue(mea <= 10^-8);
assert_checktrue(stdeviation <= 10^-8);
