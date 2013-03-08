// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Run with test_run('scicos', 'DDaskrN_Platform', ['no_check_error_output']);

// Import diagram
loadScicos();
loadXcosLibs();
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DDaskr_Platform_test.zcos"));

// Redefining messagebox() to avoid popup
function messagebox(msg, title)
 disp(title);
 disp(msg);
endfunction

for i=1:3  // 'max step size' = 10^-i, precision

 // Start by updating the clock block period (sampling)
 for j=1:length(scs_m.objs)
     if (typeof(scs_m.objs(j))=="Block" & scs_m.objs(j).gui=="CLOCK_c") then
         listClock = scs_m.objs(j).model.rpar.objs;
         for k=1:length(listClock)
             if (typeof(listClock(k))=="Block" & listClock(k).gui=="EVTDLY_c") then
                 listClock(k).graphics.exprs = [string(5*(10^-i));"0"];
                 break;
             end
         end
     end
 end

 // Modify solver + run DDaskr + save results
 scs_m.props.tol(6) = 101;         // Solver
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
 assert_checktrue(maxi <= 10^-(i+3));
 assert_checktrue(mea <= 10^-(i+3));
 assert_checktrue(stdeviation <= 10^-(i+3));

end
