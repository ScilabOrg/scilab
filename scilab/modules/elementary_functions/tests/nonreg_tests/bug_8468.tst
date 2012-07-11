// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8468 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8468
//
// <-- Short Description -->
// size with 2 input arguments did not work for hypermatrix.

M = hypermat([2 3 2 2],1:24);
ref = [2 3 2 2];
s = size(M);
if ~or(s == ref) then pause, end

s1 = size(M, 1);
if s1 <> 2 then pause, end

s2 = size(M, 2);
if s2 <> 3 then pause, end

s3 = size(M, 3);
if s3 <> 2 then pause, end

s4 = size(M, 4);
if s4 <> 2 then pause, end

s5 = size(M, 5);
if s5 <> 1 then pause, end

[n, m] = size(rand(3,2));
if n <> 3 then pause, end
if m <> 2 then pause, end

[n, m] = size(['a','b';'c','d']);
if n <> 2 then pause, end
if m <> 2 then pause, end

x = ssrand(3,2,4);
[ny, nu] = size(x);
if ny <> 3 then pause, end
if nu <> 2 then pause, end

[ny, nu] = size(ss2tf(x));
if ny <> 3 then pause, end
if nu <> 2 then pause, end

[ny, nu, nx] = size(x);
if ny <> 3 then pause, end
if nu <> 2 then pause, end
if nx <> 4 then pause, end

n = size(rand(3,2), "r");
if n <> 3 then pause, end

m = size(rand(3,2), "c");
if m <> 2 then pause, end

nm = size(rand(3,2), "*");
if nm <> 6 then pause, end
