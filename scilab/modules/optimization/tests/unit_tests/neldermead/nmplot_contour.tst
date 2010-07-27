// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Test a basic contour plot
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [1 10 100 500 1000 2000] );
drawnow();
close(f);
nm = nmplot_destroy(nm);


// Test with a function for which the column orientation matters
function [ y , index ] = myquad ( x , index )
  y = x' * x;
endfunction

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Test a basic contour plot
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",myquad);
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , linspace(0,8,20) );
drawnow();
close(f);
nm = nmplot_destroy(nm);


