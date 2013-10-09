// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// Set the seed to always get the same random numbers
grand("setsd",0);

//
// Permute some vectors, and check that the output is basically correct.
//
// With Complexes.
//
X = (2:10)';
P = grand(5,"prm",X*%i);
assert_checkequal ( isreal(P) , %f );
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , X*%i*ones(1,5) );
//
// With Integers.
//
P = grand(5,"prm",int8(X));
assert_checkequal ( typeof(P) , "int8" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int8(X)*ones(1,5) );
//
P = grand(5,"prm",int16(X));
assert_checkequal ( typeof(P) , "int16" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int16(X)*ones(1,5) );
//
P = grand(5,"prm",int32(X));
assert_checkequal ( typeof(P) , "int32" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int32(X)*ones(1,5) );
//
// With Strings.
//
X = string(X);
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "string" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , ["3" "9" "5" "7" "10" "6" "8" "2" "4"]');
//
// With Booleans.
//
X = [%f %t %t %t %t %f]';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "boolean" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , [%t %f %t %t %f %t]' );
//
// With Polynomials.
//
s = poly(0,"s");
X = [0 s 1+s 1+s^2 s^4]';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "polynomial" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , [1+s^2 1+s s s^4 0]' );
//
// With Sparses.
//
X = sparse([1 2 3 0 0 0 0 5 6])';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "sparse" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , sparse([0 0 0 3 5 6 2 1 0])' );


//
// Bug #6689
//
X = [%i 1-%i 2+3*%i].';
P = grand(4, "prm", X);
refP = [
2+3*%i  2+3*%i  %i      2+3*%i
1-%i    1-%i    2+3*%i  1-%i
%i      %i      1-%i     %i ];
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , [3 4] );
assert_checkequal ( P , refP );
