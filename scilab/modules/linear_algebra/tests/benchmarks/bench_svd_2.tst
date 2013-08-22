// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for svd 2
//==============================================================================

// <-- BENCH NB RUN : 10 -->

stacksize(30000000);

a = 0;
b = 0;

a=rand(300,300);

b     = a*a';
[c,d] = lu(b);
inv(a);

// <-- BENCH START -->
[u,s,v]=svd(a);
// <-- BENCH END -->
