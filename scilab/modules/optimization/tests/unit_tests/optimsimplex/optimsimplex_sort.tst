// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->



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
function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test optimsimplex_sort
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,3);
newsimplex = [
    1.    2.  
    3.    4.  
    5.    6.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
s1 = optimsimplex_setallfv ( s1 , [5.0 2.0 1.0]' );
s1 = optimsimplex_sort ( s1 );
ve1 = optimsimplex_getve ( s1 , 1 );
assert_equal ( ve1.n , 2 );
assert_equal ( ve1.x , [5.0 6.0] );
assert_equal ( ve1.fv , 1.0 );
ve1 = optimsimplex_getve ( s1 , 2 );
assert_equal ( ve1.n , 2 );
assert_equal ( ve1.x , [3.0 4.0] );
assert_equal ( ve1.fv , 2.0 );
ve1 = optimsimplex_getve ( s1 , 3 );
assert_equal ( ve1.n , 2 );
assert_equal ( ve1.x , [1.0 2.0] );
assert_equal ( ve1.fv , 5.0 );
s1 = optimsimplex_destroy ( s1 );

