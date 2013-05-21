// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// this is the small linear test system from UMFPACK
// whom solution must be [1;2;3;4;5]
A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6;
              0 -1 -3  2  0;
              0  0  1  0  0;
              0  4  2  0  1] );
b = [8 ; 45; -3; 3; 19];
Lup = umf_lufact(A);
x = umf_lusolve(Lup,b);

if or( (x-[ 1; 2; 3; 4; 5 ]) > %eps) then pause, end

// solve now A'x=b
x = umf_lusolve(Lup,b,"A''x=b");
if norm(A'*x - b) <> 0, then pause, end

// don't forget to clear memory with
umf_ludel(Lup)

// a real (but small)  example
// first load a sparse matrix
[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/arc130.rua");
// compute the factorisation
Lup = umf_lufact(A);
b = rand(size(A,1),1); // a random rhs
// use umf_lusolve for solving Ax=b
x = umf_lusolve(Lup,b);
firstNorm=norm(A*x - b);

// now the same thing with iterative refiment
x = umf_lusolve(Lup,b,"Ax=b",A);
secondNorm=norm(A*x - b);

if firstNorm <> secondNorm then pause, end

// don't forget to clear memory
umf_ludel(Lup)
