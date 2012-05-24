// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI TEST -->

// this is the test matrix from UMFPACK

A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);

if nnz(L) <> 9 then pause, end
if nnz(U) <> 9 then pause, end

if or(p <> [ 3; 4; 1; 5; 2 ]) then pause, end
if or(q <> [ 4; 3; 1; 5; 2 ]) then pause, end
if or(R <> [ 5; 13; 6; 1; 7 ]) then pause, end

B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
B(p,q) - L*U;  // must be a (quasi) nul matrix

if nnz(L*U) <> 13 then pause, end
if nnz(B(p,q)) <> 12 then pause, end

umf_ludel(Lup); // clear memory

// the same with a complex matrix
A = sparse( [ 2+%i  3+2*%i  0      0    0;
              3-%i  0       4+%i   0    6-3*%i; 
              0    -1+%i   -3+6*%i 2-%i 0; 
              0     0       1-5*%i 0    0; 
              0     4       2-%i   0    1] );
Lup = umf_lufact(A);
[L,U,p,q,R] = umf_luget(Lup);

if nnz(L) <> 9 then pause, end
if nnz(U) <> 9 then pause, end

if or(p <> [ 3; 4; 1; 5; 2 ]) then pause, end
if or(q <> [ 4; 3; 1; 5; 2 ]) then pause, end
if or(R <> [ 8; 18; 14; 6; 8 ]) then pause, end

B = A;
for i=1:5, B(i,:) = B(i,:)/R(i); end // apply the row scaling
B(p,q) - L*U;  // must be a (quasi) nul matrix
if nnz(B(p,q)) <> 12 then pause, end
if nnz(L*U) <> 13 then pause, end

umf_ludel(Lup) // clear memory
