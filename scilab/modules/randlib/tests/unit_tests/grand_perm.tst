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
// 1. Permute some vectors, and check that the output is basically correct.
//
X = (2:10);
P = grand_perm(1, X);
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( gsort(P,"g","i") , X );
X(2, :) = (12:20);
P = grand_perm(1, X);
refP = [
12  15  8   10  13  4  6   17  2
5   9   18  14  16  3  20  7   19 ];
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P, refP );
