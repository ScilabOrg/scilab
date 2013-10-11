// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7948 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7948
//
// <-- Short Description -->
// gsort now sorts complex numbers by real and imaginary parts thanks to "dcomplex" and "icomplex".
// It was numerically unstable when two elements had equal modulus.

y = gsort( [1-2*%i 3+%i 1+2*%i -1-2*%i 1+2*%i  1-2*%i 1+2*%i 1-2*%i 3+2*%i] , "g" , "icomplex" );
refY = [-1-2*%i 1-2*%i 1-2*%i 1-2*%i 1+2*%i 1+2*%i 1+2*%i 3+%i 3+2*%i];
assert_checkequal(y, refY);

y = gsort( [1-2*%i 3+%i 1+2*%i -1-2*%i -1+2*%i  -1+2*%i 1+2*%i 1-2*%i 3+2*%i] , "g" , "dcomplex" );
refY = [3+2*%i 3+%i 1+2*%i 1+2*%i 1-2*%i 1-2*%i -1+2*%i -1+2*%i  -1-2*%i];
assert_checkequal(y, refY);

y = gsort( [1+2*%i 3+%i 3-%i 1-2*%i 3+2*%i 4+5*%i; 4+6*%i -1-2*%i 1+2*%i 3-%i 3+%i 3-2*%i] , "g" , "icomplex" );
refY = [
-1-2*%i  1+2*%i  3-2*%i  3-%i  3+%i    4+5*%i
1-2*%i   1+2*%i  3-%i    3+%i  3+2*%i  4+6*%i ];
assert_checkequal(y, refY);
