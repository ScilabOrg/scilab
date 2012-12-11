// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run with exec("SCI/modules/xcos/examples/solvers/benchLorentz.sce");

// Import the diagram, augment the ending time and store its compilation in Info()
loadScicos();
loadXcosLibs();
importXcosDiagram("SCI/modules/xcos/demos/Lorentz.xcos");
Info = scicos_simulate(scs_m, 'nw');
tf = 10;
tolerances = scs_m.props.tol;
tolerances(1) = 10^(-13);
tolerances(2) = 10^(-13);
[%tcur, %cpr, alreadyran, needstart, needcompile, %state0] = Info(:);

solverName = ["LSodar", "CVode BDF/Newton", "CVode BDF/Functional", "CVode Adams/Newton", "CVode Adams/Functional", "Dormand-Prince", "Runge-Kutta", "implicit Runge-Kutta"];

disp("--------------------------------");
for solver = 0:7

 disp("Time for " + solverName(solver + 1) + " :");
 tolerances(6) = solver;
 
 // Here, no need to modify the step size, because it is small enough
 //if (solver >= 5) then scs_m.props.tol(7) = 10^(-3); end

 // Start the solver
 [state, t] = scicosim(%state0, 0.0, tf, %cpr.sim, 'start', tolerances);

 // Run until the end
 tic();
 [state, t] = scicosim(state, 0.0, tf, %cpr.sim, 'run', tolerances);
 t = toc();
 disp(t);

 // End the solver
 [state, t] = scicosim(state, tf, tf, %cpr.sim, 'finish', tolerances);

end
disp("--------------------------------");

