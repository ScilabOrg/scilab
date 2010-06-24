// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test on Rosenbrock test case
//

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-storehistory",%t);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [1.0;1.0], 1e-6 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , 0.0 , 1e-6 );
// Check function value history
historyfopt = neldermead_get(nm,"-historyfopt");
iterations = neldermead_get(nm,"-iterations");
sizefopt = size(historyfopt);
assert_equal ( [iterations 1], sizefopt );
// Check number of function evaluations
funevals = neldermead_get(nm,"-funevals");
assert_equal ( (funevals < 300) , %T )
// Check optimum history
historyxopt = neldermead_get(nm,"-historyxopt");
sizexopt = size(historyxopt);
assert_equal ( iterations, sizexopt );
// Check simplex history
historysimplex = neldermead_get(nm,"-historysimplex");
sizexsimplex = size(historysimplex);
assert_equal ( [iterations 3 2], sizexsimplex );
// Check fx0
fx0 = neldermead_get(nm,"-fx0");
assert_close ( fx0 , 24.2 , 10*%eps );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Cleanup
nm = neldermead_destroy(nm);

// Check that the verbose mode is functionnal
// Few iterations are necessary to check this
// Many iterations costs a lot more in time.
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",5);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-storehistory",%t);
nm = neldermead_search(nm);
status = neldermead_get(nm,"-status");
assert_equal ( status , "maxiter" );
nm = neldermead_destroy(nm);

