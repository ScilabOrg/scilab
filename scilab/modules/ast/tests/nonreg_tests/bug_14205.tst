// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14205 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14205
//
// <-- Short Description -->
// Console crash when assigning uint32 numbers to double matrix

q=1;
q(1:3,2)=uint32([1;2;3]);

assert_checkequal(q, [1 1;0 2;0 3]);
