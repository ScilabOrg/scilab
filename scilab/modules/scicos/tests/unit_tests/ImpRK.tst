// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/ImpRK.tst");
//  or test_run('scicos', 'ImpRK', ['no_check_error_output']);

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/RK_test.zcos"));

for i=2:4  // 'max step size' = 10^-i, precision

 // Start by updating the clock block period (sampling)
 if (i==2) then
  scs_m.objs(1).model.rpar.objs(2).graphics.exprs(1) = "0.01";
  else if (i==3) then
  scs_m.objs(1).model.rpar.objs(2).graphics.exprs(1) = "0.001";
   else scs_m.objs(1).model.rpar.objs(2).graphics.exprs(1) = "0.0001";
  end
 end

 // Context parameters
 for j=1:10	 // n, order
  for k=1:10

  // Modify context, solver and 'max step size' + run RK + save results
  scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];  // Context
  scs_m.props.tol(7) = 10^(-i); scs_m.props.tol(6) = 4;					// 'max step size' + solver
  try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()) end; // RK
  rkval = res.values;  		// Results
  time = res.time;  		// Time

  // Modify context, solver and 'max step size' + run CVode + save results
  scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];
  scs_m.props.tol(7) = 1E-15; scs_m.props.tol(6) = 0;
  try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()) end;
  cvval = res.values;

  // Compare results
  compa = abs(rkval-cvval);

  // Extract mean, standard deviation, maximum
  mea = mean(compa);
  [maxi, indexMaxi] = max(compa);
  stdeviation = st_deviation(compa);

  // Verifying closeness of the results
  assert_checktrue(maxi <= 10^-(i+2));
  assert_checktrue(mea <= 10^-(i+2));
  assert_checktrue(stdeviation <= 10^-(i+2));

  end
 end
end
