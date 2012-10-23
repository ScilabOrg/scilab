// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/rk.tst");
//  or test_run('scicos', 'rk', ['no_check_error_output']);

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/AArk.zcos"));

for i=2:4  // abstol = 10^-i, precision

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

  // Modify context, solver and abstol + run RK + save results
  scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];  // Context
  scs_m.props.tol(1) = 10^(-i); scs_m.props.tol(6) = 4;					// abstol + solver
  xcos_simulate(scs_m, 4);  // RK
  rkval = res.values;  		// Results
  time = res.time;  		// Time

  // Modify context, solver and abstol + run CVode + save results
  scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];
  scs_m.props.tol(1) = 1E-15; scs_m.props.tol(6) = 0;
  xcos_simulate(scs_m, 4);
  cvval = res.values;

  // Compare results
  compa = abs(rkval-cvval);

  // Extract mean, standard deviation, maximum
  mea = mean(compa);
  [maxi, indexMaxi] = max(compa);
  stdeviation = st_deviation(compa);

  // Verifying closeness of the results
  if (maxi >= 10^-(i+2)) then pause, end

  // Vectors [mean, standard deviation] and [max, time_max]
  vec = [mea, stdeviation];
  maxiT = [maxi time(indexMaxi)];

  // Display
  //disp("[mean, standard deviation] :");
  //disp(vec);
  //disp("[maximum difference, at time] :");
  //disp(maxiT);

  end
 end
end
