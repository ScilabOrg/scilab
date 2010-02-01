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
function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test with x0 only
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] );
computed = optimsimplex_getall ( s1 );
expected = [
    0.  -1.2    1.  
    0.  -0.2    1.  
    0.  -1.2    2.  
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with function only
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2  -1.2    1.  
    93.6  -0.2    1.  
    36.2  -1.2    2.  
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a scalar length
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock , 2.0 );
computed = optimsimplex_getall ( s1 );
expected = [
    24.2   -1.2    1.  
    13.      0.8    1.  
    248.2  -1.2    3.  
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a vector length
//
s1 = optimsimplex_new ( "axes" , [-1.2 1.0] , rosenbrock , [1.0 2.0] );
computed = optimsimplex_getall ( s1 );
expected = [
24.2 -1.2 1.
93.6 -0.2 1.
248.2 -1.2 3.
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with a scalar length and an additionnal object
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
[ s1 , myobj ] = optimsimplex_new ( "axes" , [-1.2 1.0] , mycostf , 1.0, myobj );
computed = optimsimplex_getall ( s1 );
expected = [
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
assert_close ( computed , expected , %eps );
assert_equal ( myobj.nb , 3 );
nbve = optimsimplex_getnbve ( s1 );
assert_equal ( nbve , 3 );
s1 = optimsimplex_destroy ( s1 );

// Test with a unconsistent x0 and len
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2] , rosenbrock , [1 2 3] )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_axes: The len vector is not consistent with the x0 point. Current shape of x0 is 1 x 2 while current shape of len is 1 x 3.";
assert_equal ( computed , expected );

// Test with a unconsistent x0
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2;3 4] , rosenbrock )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_axes: The x0 vector is expected to be a row matrix, but current shape is 2 x 2.";
assert_equal ( computed , expected );

// Test with a unconsistent len
cmd = "newobj = optimsimplex_new ( ""axes"" , [1 2] , rosenbrock , ""foo"" )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "assert_typereal: Expected real variable for variable len at input #1, but got string instead.";
assert_equal ( computed , expected );

