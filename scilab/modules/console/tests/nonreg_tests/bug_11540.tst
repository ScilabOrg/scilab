// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 11540 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11540
//
// <-- Short Description -->
// llines(X) stopped display after X lines and can't be continued

lines(10)
rand(100,10)

// enter what you want, all should be as expected
