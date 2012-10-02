// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

// Execute with exec("SCI/modules/scicos/tests/unit_tests/rk10-3.tst");

for i=1:10	// n, order
 for j=1:10

// Run RK + modify context + save results
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/ABrk10-3.zcos"));
scs_m.props.context = ["n = "+string(i)+";" "per = "+string(j)+";"];
xcos_simulate(scs_m, 4);
rkval = rk.values;
time = rk.time;

// Run CVode + modify context + save results
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/ABcv10-3.zcos"));
scs_m.props.context = ["n = "+string(i)+";" "per = "+string(j)+";"];
xcos_simulate(scs_m, 4);
cvval = cv.values;

// Compare results
compa = abs(rkval-cvval);

// Extract mean, standard deviation, maximum
mea = mean(compa);
[maxi, indexMaxi] = max(compa);
stdeviation = st_deviation(compa);

// Verifying closeness of the results
if (maxi >= 0.0001) then pause, end

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
