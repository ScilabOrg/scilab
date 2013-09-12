// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
// <-- CLI SHELL MODE -->


x = factorial ( 0 );
assert_checkequal ( x , 1 );
x = factorial ( 1 );
assert_checkequal ( x , 1 );
x = factorial ( 2 );
assert_checkequal ( x , 2 );
x = factorial ( [1 2 3 4] );
assert_checkequal ( x , [1 2 6 24] );
x = factorial ( 170 );
assert_checkalmostequal ( x , 7.25741561530799896739e306 , 1000 * %eps );
// Test with a matrix
n = [
1 2 3
4 5 6
7 8 9
];
x = factorial ( n );
e = [
1.       2.        6.
24.      120.      720.
5040.    40320.    362880.
];
assert_checkequal ( x , e );
// Test with an hypermatrix
clear n;
clear e;
n(1,1,1,1:2)=[1 2];
x = factorial ( n );
e(1,1,1,1:2)=[1 2];
assert_checkequal ( x , e );
clear n;
clear e;

// Integers
assert_checkequal ( factorial([int8(0) int8(1) int8(2)]) , [int8(1) int8(1) int8(2)] );
assert_checkequal ( factorial([uint8(0) uint8(1) uint8(2)]) , [uint8(1) uint8(1) uint8(2)] );
assert_checkequal ( factorial([int16(0) int16(1) int16(2)]) , [int16(1) int16(1) int16(2)] );
assert_checkequal ( factorial([uint16(0) uint16(1) uint16(2)]) , [uint16(1) uint16(1) uint16(2)] );
assert_checkequal ( factorial([int32(0) int32(1) int32(2)]) , [int32(1) int32(1) int32(2)] );
assert_checkequal ( factorial([uint32(0) uint32(1) uint32(2)]) , [uint32(1) uint32(1) uint32(2)] );
x1 = factorial ( int8([1 2 3 4]) );
x2 = factorial ( int16([1 2 3 4]) );
x3 = factorial ( int32([1 2 3 4]) );
x4 = factorial ( uint8([1 2 3 4]) );
x5 = factorial ( uint16([1 2 3 4]) );
x6 = factorial ( uint32([1 2 3 4]) );
assert_checkequal ( x1 , int8([1 2 6 24]) );
assert_checkequal ( x2 , int16([1 2 6 24]) );
assert_checkequal ( x3 , int32([1 2 6 24]) );
assert_checkequal ( x4 , uint8([1 2 6 24]) );
assert_checkequal ( x5 , uint16([1 2 6 24]) );
assert_checkequal ( x6 , uint32([1 2 6 24]) );
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Result will exceed type limitation"), "factorial", 1);
assert_checkerror("factorial(int8(50));", refMsg);
assert_checkerror("factorial(int16(50));", refMsg);
assert_checkerror("factorial(int32(50));", refMsg);
assert_checkerror("factorial(uint8(50));", refMsg);
assert_checkerror("factorial(uint16(50));", refMsg);
assert_checkerror("factorial(uint32(50));", refMsg);
