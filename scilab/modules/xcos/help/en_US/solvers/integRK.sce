// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run with exec("SCI/modules/xcos/help/en_US/solvers/integRK.sce");

// Import the diagram and augment the ending time
importXcosDiagram("SCI/modules/xcos/help/examples/continuous_pal/en_US/Integral.xcos");
scs_m.props.tf = 50000;

// BDF / Newton
// Select the solver
scs_m.props.tol(6) = 0;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for BDF / Newton :");

// BDF / Functional
// Select the solver
scs_m.props.tol(6) = 1;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for BDF / Functional :");

// Adams / Functional
// Select the solver
scs_m.props.tol(6) = 3;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for Adams / Functional :");

// Adams / Newton
// Select the solver
scs_m.props.tol(6) = 2;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer();
disp(t, "Time for Adams / Newton :");

// Runge-Kutta
// Select the solver and set abstol to 10^-2
scs_m.props.tol(6) = 4;
scs_m.props.tol(1) = 0.01;
// Start the timer, launch the simulation and display time
timer();
xcos_simulate(scs_m, 4);
t = timer;
disp(t, "Time for Runge-Kutta :");
