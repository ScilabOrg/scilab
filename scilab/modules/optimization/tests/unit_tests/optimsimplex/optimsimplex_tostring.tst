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

// Test with 3 vertices
s1 = optimsimplex_new ();
simplex = [
24. -2.0 1.0
93. -1.0 3.0
36. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
str = string ( s1 );
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/3 : fv=24, x=-2 1"
"Vertex #2/3 : fv=93, x=-1 3"
"Vertex #3/3 : fv=36, x=-3 2"
];
assert_equal ( str , expected );
s1 = optimsimplex_destroy ( s1 );

// Test with 4 vertices
s1 = optimsimplex_new ();
simplex = [
24. -2.0 1.0
93. -1.0 3.0
36. -3.0 2.0
36. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
str = string ( s1 );
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/4 : fv=24, x=-2 1"
"Vertex #2/4 : fv=93, x=-1 3"
"Vertex #3/4 : fv=36, x=-3 2"
"Vertex #4/4 : fv=36, x=-3 2" 
];
assert_equal ( str , expected );
s1 = optimsimplex_destroy ( s1 );

