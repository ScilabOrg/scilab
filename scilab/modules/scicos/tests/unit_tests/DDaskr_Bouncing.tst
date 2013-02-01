// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/DDaskr_Bouncing.tst");
//  or test_run('scicos', 'DDaskr_Bouncing', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DDaskr_Bouncing_test.zcos"));

// Redefining messagebox() to avoid popup
function messagebox(msg, title)
 disp(title);
 disp(msg);
endfunction

// Start by updating the clock block period (sampling)
scs_m.objs(1).model.rpar.objs(2).graphics.exprs = [string(5*(10^-3));"0"];

// Modify solver + run DDaskr + save results
scs_m.props.tol(6) = 101;           // Solver
//scs_m.props.tol(1) = 1.0e-10;      // abstol
//scs_m.props.tol(2) = 1.0e-10;      // reltol
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end  // DDaskr
ddaskrval = res.values;   // Results
time = res.time;          // Time

// Modify solver + run IDA + save results
scs_m.props.tol(6) = 100;         // Solver
try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end  // IDA
idaval = res.values;  // Results

// Compare results
compa = abs(ddaskrval-idaval);

// Extract mean, standard deviation, maximum
mea = mean(compa);
[maxi, indexMaxi] = max(compa);
stdeviation = st_deviation(compa);

// Verifying closeness of the results
assert_checktrue(maxi <= 10^-(12));
assert_checktrue(mea <= 10^-(12));
assert_checktrue(stdeviation <= 10^-(12));
