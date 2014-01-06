// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- XCOS TEST -->
//

ilib_verbose(0); //to remove ilib_* traces

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Solvers/DAE/Platform.zcos"));

// Redefining messagebox() to avoid popup
prot = funcprot();
funcprot(0);
function messagebox(msg, msg_title)
endfunction

for i=1:3  // 'max step size' = 10^-i, precision

    // Start by updating the clock block period (sampling)
    Context.per = 5*10^-i;

    // Modify solver + run DDaskr + save results
    scs_m.props.tol(6) = 101;     // Solver
    scicos_simulate(scs_m, Context); // DDaskr
    ddaskrval = res.values;       // Results

    // Modify solver + run IDA + save results
    scs_m.props.tol(6) = 100;     // Solver
    scicos_simulate(scs_m, Context); // IDA
    idaval = res.values;          // Results

    // Compare results
    compa = abs(ddaskrval-idaval);

    // Extract mean, standard deviation, maximum
    mea = mean(compa);
    [maxi, indexMaxi] = max(compa);
    stdeviation = stdev(compa);

    // Verifying closeness of the results
    assert_checktrue(maxi <= 5*10^-(i+3));
    assert_checktrue(mea <= 5*10^-(i+3));
    assert_checktrue(stdeviation <= 5*10^-(i+3));

end
funcprot(prot);
