// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction


//
// myoutputcmd --
//  This command is called back by the Nelder-Mead
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * simplex : the simplex, as a simplex object
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//
function myoutputcmd ( state , data )
  iter = data.iteration
  if ( state == "init" ) then
    mprintf ( "=================================\n");
    mprintf ( "Initialization\n");
  elseif ( state == "done" ) then
    mprintf ( "=================================\n");
    mprintf ( "End of Optimization\n");
  end
  fc = data.funccount
  fval = data.fval
  x = data.x
  simplex = data.simplex
  // Simplex is a data structure, which can be managed
  // by the optimsimplex class.
  ssize = optimsimplex_size ( simplex )
  mprintf ( "Iteration #%d, Feval #%d, Fval = %e, x = %s, Size = %e\n", iter, fc, fval, strcat(string(x)," "), ssize);
endfunction


nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);
mprintf("End of demo.\n");

//
// Load this script into the editor
//
filename = 'neldermead_outputcmd.sce';
dname = get_absolute_file_path(filename);
editor ( dname + filename );

