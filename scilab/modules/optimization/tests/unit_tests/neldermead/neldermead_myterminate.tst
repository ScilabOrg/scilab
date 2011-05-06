// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
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



function stop = myoutputcmd ( state , data )
  simplex = data.simplex
  ssize = optimsimplex_size ( simplex , "sigmaplus" );
  if ( ssize < 1.e-4 ) then
    stop = %t;
    status = "mysize";
  else
    stop = %f
  end
endfunction

function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction




//
// Test with my own termination criteria 
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",400);
nm = neldermead_configure(nm,"-method","variable");
// Disable default terminations
nm = neldermead_configure(nm,"-tolxmethod",%f);
nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [1.0 1.0]', 1e-3 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , 0.0 , 1e-5 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "userstop" );
// Check simplex size
simplex = neldermead_get(nm,"-simplexopt");
ssize = optimsimplex_size ( simplex , "sigmaplus" );
assert_equal ( ssize<1.e-4 , %t );
// Check function evaluations
// If not user-termination, then funevals = 401, i.e.
// the maximum.
funevals = neldermead_get(nm,"-funevals");
assert_equal ( funevals<200 , %t );
nm = neldermead_destroy(nm);

