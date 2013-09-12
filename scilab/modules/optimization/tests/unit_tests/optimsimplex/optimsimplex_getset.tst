// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test optimsimplex_new, optimsimplex_destroy, optimsimplex_setn, optimsimplex_setx, optimsimplex_setfv,
// optimsimplex_getn, optimsimplex_getx, optimsimplex_getfv
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,3);
s1 = optimsimplex_setx(s1,1,[0.0 0.0]);
s1 = optimsimplex_setfv(s1,1,12.0);
s1 = optimsimplex_setx(s1,2,[1.0 0.0]);
s1 = optimsimplex_setfv(s1,2,13.0);
s1 = optimsimplex_setx(s1,3,[0.0 2.0]);
s1 = optimsimplex_setfv(s1,3,14.0);
// Now check the data
computed = optimsimplex_getn(s1);
assert_checkequal ( computed , 2 );
computed = optimsimplex_getx( s1 , 1);
assert_checkequal ( computed , [0.0 0.0] );
computed = optimsimplex_getx( s1 , 2);
assert_checkequal ( computed , [1.0 0.0] );
computed = optimsimplex_getx( s1 , 3);
assert_checkequal ( computed , [0.0 2.0] );
computed = optimsimplex_getfv( s1 , 1);
assert_checkequal ( computed , 12.0 );
computed = optimsimplex_getfv( s1 , 2);
assert_checkequal ( computed , 13.0 );
computed = optimsimplex_getfv( s1 , 3);
assert_checkequal ( computed , 14.0 );
// Print the simplex
disp ( s1 );
// We are done !
s1 = optimsimplex_destroy(s1);

//
// Test optimsimplex_setallx, optimsimplex_setallfv, optimsimplex_getallx, optimsimplex_getallfv.
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,3);
s1 = optimsimplex_setx(s1,1,[0.0 0.0]);
s1 = optimsimplex_setfv(s1,1,12.0);
s1 = optimsimplex_setx(s1,2,[1.0 0.0]);
s1 = optimsimplex_setfv(s1,2,13.0);
s1 = optimsimplex_setx(s1,3,[0.0 2.0]);
s1 = optimsimplex_setfv(s1,3,14.0);
// Now check the data
computed = optimsimplex_getallx (s1);
expected = [
0.0 0.0
1.0 0.0 
0.0 2.0];
assert_checkequal ( computed , expected );
computed = optimsimplex_getallfv (s1);
assert_checkequal ( computed , [12.0 13.0 14.0]' );
// setallx, setallfv
newsimplex = [
1.0 2.0
3.0 4.0 
5.0 6.0];
s1 = optimsimplex_setallx ( s1 , newsimplex );
computed = optimsimplex_getallx (s1);
assert_checkequal ( computed , newsimplex );
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0]' );
computed = optimsimplex_getallfv (s1);
assert_checkequal ( computed , [3.0 4.0 5.0]' );
s1 = optimsimplex_destroy(s1);

//
// Test optimsimplex_setve, optimsimplex_getve
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve(s1,3);
s1 = optimsimplex_setve ( s1 , 1 , 13.0 , [0.0 0.0] );
s1 = optimsimplex_setve ( s1 , 2 , 14.0 , [1.0 0.0] );
s1 = optimsimplex_setve ( s1 , 3 , 15.0 , [0.0 2.0] );
computed = optimsimplex_getallx ( s1 );
expected = [
0.0 0.0
1.0 0.0  
0.0 2.0];
assert_checkequal ( computed , expected );
computed = optimsimplex_getallfv ( s1 );
assert_checkequal ( computed , [13.0 14.0 15.0]' );
ve1 = optimsimplex_getve ( s1 , 1 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [0.0 0.0] );
assert_checkequal ( ve1.fv , 13.0 );
ve2 = optimsimplex_getve ( s1 , 2 );
assert_checkequal ( ve2.n , 2 );
assert_checkequal ( ve2.x , [1.0 0.0] );
assert_checkequal ( ve2.fv , 14.0 );
ve3 = optimsimplex_getve ( s1 , 3 );
assert_checkequal ( ve3.n , 2 );
assert_checkequal ( ve3.x , [0.0 2.0] );
assert_checkequal ( ve3.fv , 15.0 );
s1 = optimsimplex_destroy(s1);
//
// optimsimplex_getall, optimsimplex_setall
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve(s1,3);
s1 = optimsimplex_setve ( s1 , 1 , 13.0 , [0.0 0.0] );
s1 = optimsimplex_setve ( s1 , 2 , 14.0 , [1.0 0.0] );
s1 = optimsimplex_setve ( s1 , 3 , 15.0 , [0.0 2.0] );
computed = optimsimplex_getall ( s1 );
expected = [
    13.    0.    0.  
    14.    1.    0.  
    15.    0.    2.  
];
assert_checkequal ( computed , expected );
simplex = [
10.0 1.0 2.0
11.0 3.0 4.0
12.0 5.0 6.0
];
s1 = optimsimplex_setall ( s1 , simplex );
ve1 = optimsimplex_getve ( s1 , 1 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [1.0 2.0] );
assert_checkequal ( ve1.fv , 10.0 );
ve1 = optimsimplex_getve ( s1 , 2 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [3.0 4.0] );
assert_checkequal ( ve1.fv , 11.0 );
ve1 = optimsimplex_getve ( s1 , 3 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [5.0 6.0] );
assert_checkequal ( ve1.fv , 12.0 );
s1 = optimsimplex_destroy(s1);
//
// Test setall with wrong simplex : 
// there are 3 parameters => 4 columns
// but there is only 3 vertices, while the minimum would be 4.
//
s1 = optimsimplex_new ();
simplex = [
10.0 1.0 2.0 12.0
11.0 3.0 4.0 12.0
12.0 5.0 6.0 12.0
];
cmd = "s1 = optimsimplex_setall ( s1 , simplex );";
assert_checkerror ( cmd , "%s: The number of vertices (i.e. the number of rows) is %d which is smaller than the number of columns %d (i.e. n+1)." , [], "optimsimplex_setall",3,4);
s1 = optimsimplex_destroy(s1);

//
// optimsimplex_getall, optimsimplex_setall with 5 vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve(s1, 5 );
s1 = optimsimplex_setve ( s1 , 1 , 13.0 , [0.0 0.0] );
s1 = optimsimplex_setve ( s1 , 2 , 14.0 , [1.0 0.0] );
s1 = optimsimplex_setve ( s1 , 3 , 15.0 , [0.0 2.0] );
s1 = optimsimplex_setve ( s1 , 4 , 16.0 , [1.0 3.0] );
s1 = optimsimplex_setve ( s1 , 5 , 17.0 , [2.0 4.0] );
computed = optimsimplex_getall ( s1 );
expected = [
    13.    0.    0.  
    14.    1.    0.  
    15.    0.    2.  
    16.    1.    3.  
    17.    2.    4.  
];
assert_checkequal ( computed , expected );
simplex = [
10.0 1.0 2.0
11.0 3.0 4.0
12.0 5.0 6.0
13.0 6.0 7.0
14.0 7.0 8.0
];
s1 = optimsimplex_setall ( s1 , simplex );
ve1 = optimsimplex_getve ( s1 , 1 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [1.0 2.0] );
assert_checkequal ( ve1.fv , 10.0 );
ve1 = optimsimplex_getve ( s1 , 2 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [3.0 4.0] );
assert_checkequal ( ve1.fv , 11.0 );
ve1 = optimsimplex_getve ( s1 , 3 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [5.0 6.0] );
assert_checkequal ( ve1.fv , 12.0 );
ve1 = optimsimplex_getve ( s1 , 4 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [6.0 7.0] );
assert_checkequal ( ve1.fv , 13.0 );
ve1 = optimsimplex_getve ( s1 , 5 );
assert_checkequal ( ve1.n , 2 );
assert_checkequal ( ve1.x , [7.0 8.0] );
assert_checkequal ( ve1.fv , 14.0 );
s1 = optimsimplex_destroy(s1);
//
// Test optimsimplex_setallx, optimsimplex_setallfv, optimsimplex_getallx, optimsimplex_getallfv with 5 vertices
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn(s1,2);
s1 = optimsimplex_setnbve(s1,5);
s1 = optimsimplex_setx  ( s1 , 1 , [0.0 0.0]);
s1 = optimsimplex_setfv ( s1 , 1 , 12.0);
s1 = optimsimplex_setx  ( s1 , 2 , [1.0 0.0]);
s1 = optimsimplex_setfv ( s1 , 2 , 13.0);
s1 = optimsimplex_setx  ( s1 , 3 , [0.0 2.0]);
s1 = optimsimplex_setfv ( s1 , 3 , 14.0);
s1 = optimsimplex_setx  ( s1 , 4 , [1.0 3.0]);
s1 = optimsimplex_setfv ( s1 , 4 , 15.0);
s1 = optimsimplex_setx  ( s1 , 5 , [2.0 4.0]);
s1 = optimsimplex_setfv ( s1 , 5 , 16.0);
// Now check the data
computed = optimsimplex_getallx (s1);
expected = [
    0.    0.  
    1.    0.  
    0.    2.  
    1.    3.  
    2.    4.  
];
assert_checkequal ( computed , expected );
computed = optimsimplex_getallfv (s1);
assert_checkequal ( computed , [12.0 13.0 14.0 15.0 16.0]' );
// setallx, setallfv
newsimplex = [
    1.    2.   
    3.    4.   
    5.    6.   
    7.    8.   
    9.    10.  
];
s1 = optimsimplex_setallx ( s1 , newsimplex );
computed = optimsimplex_getallx (s1);
assert_checkequal ( computed , newsimplex );
s1 = optimsimplex_setallfv ( s1 , [3.0 4.0 5.0 6.0 7.0]' );
computed = optimsimplex_getallfv (s1);
assert_checkequal ( computed , [3.0 4.0 5.0 6.0 7.0]' );
s1 = optimsimplex_destroy(s1);

