// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for modulo() function
// =============================================================================

assert_checkerror("modulo()", msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"modulo", 2));
assert_checkerror("modulo(''a'',1)", msprintf(_("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n"), "modulo", 1));
assert_checkerror("modulo(1,''a'')", msprintf(_("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n"), "modulo", 2));
assert_checkerror("modulo(%pi+%i,1)", msprintf(_("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n"), "modulo", 1));
assert_checkerror("modulo(1,1+%i)", msprintf(_("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n"), "modulo", 2));

n=[1,2,10,15];
m=[2,2,3,5];
r = modulo(n,m);
assert_checkequal(r, [1,0,1,0]);

n = 100 * rand(1,100);
m = 100 * rand(1,100);
r = modulo(n,m);
computed_r = n - m .* int (n ./ m);
assert_checkequal(r, computed_r);

assert_checkequal(modulo(%z^2,%z),0);
// Encoded integers
assert_checkequal(modulo( int8(-123), int16(-7)), int8(-4));
assert_checkequal(pmodulo(int8(-123), int16(-7)), int8(3));
assert_checkequal(modulo( int8(-123), int16([-7 5])),int8([-4,-3]));
assert_checkequal(pmodulo(int8(-123), int16([-7 5])),int8([3,2]));
assert_checkequal(modulo( int8([-123 51]), int16(-7)),  int8([-4,2]));
assert_checkequal(pmodulo(int8([-123 51]), int16(-7)),int8([3,2]));
assert_checkequal(modulo( int8([-123 51]), int16([-7 5])),int8([-4,1]) );
assert_checkequal(pmodulo(int8([-123 51]), int16([-7 5])),  int8([3,1]));

// Hypermatrices
m = hypermat([2 2 2],1:8);
n = hypermat([2 2 2],-9:-2)
a=[1 3;2 4];a(:,:,2)=[0 2; 1 3];
assert_checkequal(modulo(m, 5), a );
assert_checkequal(pmodulo(m,5), a);
a=[6 2;3 3];a(:,:,2)=[1 0; 3 1];
assert_checkequal(modulo(51, n), a);
assert_checkequal(pmodulo(51,n), a);
a=[1 3;2 4];a(:,:,2)=[0 1; 2 0];
assert_checkequal(modulo(m, n), a);
assert_checkequal(pmodulo(m,n), a);

// Polynomials
assert_checkequal(modulo( %z^2+1, %z), 1);
assert_checkequal(pmodulo(%z^2+1, %z), 1);


assert_checkequal(modulo(     -9 ,     [-7 5]) ,[-2 -4]);
assert_checkequal(modulo(int8(-9),int8([-7 5])),int8([-2 -4]));
assert_checkequal(modulo(      9 ,     [-7 5]) ,[2 4]);
assert_checkequal(modulo( int8(9),int8([-7 5])),int8([2 4]));

assert_checkequal(pmodulo(     -9,      [-7 5]) ,[5 1]);
assert_checkequal(pmodulo(int8(-9),int8([-7 5])),int8([5 1]));
assert_checkequal(pmodulo(      9,      [-7 5]) ,[2 4]);
assert_checkequal(pmodulo( int8(9),int8([-7 5])),int8([2 4]));

