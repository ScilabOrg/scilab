
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/DoPri.tst");
//  or test_run('scicos', 'DoPri', ['no_check_error_output']);

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/DoPri_test.zcos"));

for i=2:4  // abstol = 5*10^-i, precision

 // Start by updating the clock block period (sampling)
 scs_m.objs(8).model.rpar(1) = 5*10^(-i);
 scs_m.objs(9).model.rpar(1) = 5*10^(-i);

 // Context parameters
 for j=1:2     // n, order
  for k=1:2

   // Modify context, solver and abstol + run DoPri + save results
   //scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];	// Context
   scs_m.props.tol(1) = 5*10^(-i); scs_m.props.tol(6) = 4;					// abstol + solver
   try xcos_simulate(scs_m, 4); catch end;	// DoPri
   doprival = res.values;	// Results
   time = res.time;			// Time

   // Modify context, solver and abstol + run CVode + save results
   //scs_m.props.context = ["n = "+string(j)+";" "per = "+string(k)+";"];
   scs_m.props.tol(1) = 1E-15; scs_m.props.tol(6) = 0;
   try xcos_simulate(scs_m, 4); catch end;
   cvval = res.values;

   // Compare results
   compa = abs(doprival-cvval);

   // Extract mean, standard deviation, maximum
   mea = mean(compa);
   [maxi, indexMaxi] = max(compa);
   stdeviation = st_deviation(compa);

disp(maxi)

   // Verifying closeness of the results
   assert_checktrue(maxi <= 10^-(i+2));
   assert_checktrue(mea <= 10^-(i+2));
   assert_checktrue(stdeviation <= 10^-(i+2));

  end
 end
end
