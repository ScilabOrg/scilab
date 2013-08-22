// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

A = sparse( [ 2  3  0  0  0;
3  0  4  0  6;
0 -1 -3  2  0;
0  0  1  0  0;
0  4  2  0  1] );
b = [8 ; 45; -3; 3; 19];
x = umfpack(A,"\",b);


if norm(x - [1 2 3 4 5]')  < %eps then pause,end

// test the other form x A = b
b = [8  20  13  6  17];
x = umfpack(b,"/",A);   // solution must be [1 2 3 4 5]

if norm(x - [1 2 3 4 5])  < %eps then pause,end

// test multiple rhs
b = rand(5,3);
x = umfpack(A,"\",b);

if norm(A*x - b) < %eps  then pause,end

// test multiple rhs for x A = b
b = rand(3,5);
x = umfpack(b,"/",A);
if norm(x*A - b) > %eps  then pause,end

// solve a complex system
A = sparse( [ 2+%i  3+2*%i  0      0    0;
3-%i  0       4+%i   0    6-3*%i;
0    -1+%i   -3+6*%i 2-%i 0;
0     0       1-5*%i 0    0;
0     4       2-%i   0    1] );
b = [ 3+13*%i ; 58+32*%i ; -19+13*%i ; 18-12*%i ; 22+16*%i ];
x = umfpack(A,"\",b)  // x must be [1+i; 2+2i; 3+3i; 4 + 4i; 5+5i]

if norm(x - [1+%i; 2+2*%i; 3+3*%i; 4 + 4*%i; 5+5*%i]) < %eps then pause,end


A = sparse( [ 2  3  0  0  0;
3  0  4  0  6;
0 -1 -3  2  0;
0  0  1  0  0;
0  4  2  0  1] );
Lup = umf_lufact(A);
[OK, nrow, ncol, lnz, unz, udiag_nz, it] = umf_luinfo(Lup);  // OK must be %t, nrow=ncol = 5,
[L,U,p,q,R] = umf_luget(Lup);
nnz(L)  // must be equal to lnz
if nnz(L) <> lnz then pause,end

nnz(U)  // must be equal to unz
if nnz(U) <> unz then pause,end

umf_ludel(Lup); // clear memory



// this is the test matrix from UMFPACK
A = sparse( [ 2  3  0  0  0;
3  0  4  0  6;
0 -1 -3  2  0;
0  0  1  0  0;
0  4  2  0  1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);
B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
// must be a (quasi) nul matrix
if norm(B(p,q) - L*U) > %eps then pause,end
umf_ludel(Lup);// clear memory


// the same with a complex matrix
A = sparse( [ 2+%i  3+2*%i  0      0    0;
3-%i  0       4+%i   0    6-3*%i;
0    -1+%i   -3+6*%i 2-%i 0;
0     0       1-5*%i 0    0;
0     4       2-%i   0    1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);
B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
// must be a (quasi) nul matrix
if norm(B(p,q) - L*U) > %eps then pause,end
umf_ludel(Lup); // clear memory
