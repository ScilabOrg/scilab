// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12862
//
// <-- Short Description -->
// after bug 9002, input arguments of splin must be real.

x = 1:5;
y = x;
res = intsplin(x, y);
assert_checkequal(res, 12);
res = intsplin(x,%i*y);
assert_checkequal(res, 12*%i);
