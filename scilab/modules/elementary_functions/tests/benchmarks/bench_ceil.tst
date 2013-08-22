// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for ceil function
//==============================================================================

// <-- BENCH NB RUN : 10000 -->

stacksize(30000000);

// <-- BENCH START -->
a = ceil(1000 * rand(700,1,"u"));
// <-- BENCH END -->
