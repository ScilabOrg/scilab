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
// Test optimsimplex_new with a matrix of coordinates
//
coords = [
    0.    0.  
    1.    0.  
    0.    1.  
];
s1 = optimsimplex_new ( coords );
computed = optimsimplex_getallx ( s1 );
expected = [
    0.    0.  
    1.    0.  
    0.    1.  
];
assert_equal ( computed , expected );
computed = optimsimplex_getn(s1);
assert_equal ( computed , 2 );
computed = optimsimplex_getnbve (s1);
assert_equal ( computed , 3 );
s1 = optimsimplex_destroy(s1);

//
// Test optimsimplex_new with a matrix of coordinates and 
// a function
//
coords = [
    0.    0.  
    1.    0.  
    0.    1.  
];
s1 = optimsimplex_new ( coords , rosenbrock );
computed = optimsimplex_getall ( s1 );
expected = [
    1.      0.    0.  
    100.    1.    0.  
    101.    0.    1.  
];
assert_equal ( computed , expected );
computed = optimsimplex_getn(s1);
assert_equal ( computed , 2 );
computed = optimsimplex_getnbve (s1);
assert_equal ( computed , 3 );
s1 = optimsimplex_destroy(s1);

//
// Test with a function which has an additional argument
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
coords = [
    0.    0.  
    1.    0.  
    0.    1.  
];
[ s1 , myobj ] = optimsimplex_new ( coords , mycostf , myobj );
computed = optimsimplex_getall ( s1 );
expected = [
    1.      0.    0.  
    100.    1.    0.  
    101.    0.    1.  
];
assert_equal ( computed , expected );
assert_equal ( myobj.nb , 3 );
computed = optimsimplex_getn(s1);
assert_equal ( computed , 2 );
computed = optimsimplex_getnbve (s1);
assert_equal ( computed , 3 );
s1 = optimsimplex_destroy ( s1 );
//
// Test optimsimplex_new with a matrix of coordinates made of k=5 vertices
//
coords = [
    0.    0.  
    1.    0.  
    0.    1.  
    1.    1.  
    2.    2.  
];
s1 = optimsimplex_new ( coords , rosenbrock );
computed = optimsimplex_getall ( s1 );
expected = [
    1.      0.    0.  
    100.    1.    0.  
    101.    0.    1.  
    0.      1.    1.  
    401.    2.    2.  
];
assert_equal ( computed , expected );
computed = optimsimplex_getn(s1);
assert_equal ( computed , 2 );
computed = optimsimplex_getnbve (s1);
assert_equal ( computed , 5 );
s1 = optimsimplex_destroy(s1);

