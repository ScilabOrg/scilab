// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for weekday function
//==============================================================================

today = datenum();

// <-- BENCH START -->
[N,S] = weekday(today);
// <-- BENCH END -->

