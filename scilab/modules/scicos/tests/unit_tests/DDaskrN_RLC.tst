// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Run with test_run('scicos', 'DDaskrN_RLC', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DDaskr_RLC_test.zcos"));

// Redefining messagebox() to avoid popup
function messagebox(msg, title)
 disp(title);
 disp(msg);
endfunction

for i=2:3

    // Start by updating the clock block period (sampling)
    scs_m.objs(1).model.rpar.objs(2).graphics.exprs = [string(5*(10^-i));"0"];

    // Modify solver + run DDaskr + save results
    scs_m.props.tol(6) = 101;           // Solver
    try xcos_simulate(scs_m, 4); catch disp(lasterror()); end  // DDaskr
    ddaskrval = res.values;   // Results
    time = res.time;          // Time

    // Modify solver + run IDA + save results
    scs_m.props.tol(6) = 100;         // Solver
    try xcos_simulate(scs_m, 4); catch disp(lasterror()); end  // IDA
    idaval = res.values;  // Results

    // Compare results
    compa = abs(ddaskrval-idaval);

    // Extract mean, standard deviation, maximum
    mea = mean(compa);
    [maxi, indexMaxi] = max(compa);
    stdeviation = st_deviation(compa);

    // Verifying closeness of the results
    assert_checktrue(maxi <= 10^-(i+4));
    assert_checktrue(mea <= 10^-(i+4));
    assert_checktrue(stdeviation <= 10^-(i+4));

end
