// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9976
//
// <-- Short Description -->
//    The inverse cdfbet function returned wrong value instead of 0
// =============================================================================

a = 1;
b = 2;
p = 0;
q = 1-p;
x = cdfbet("XY",a,b,p,q);
assert_checkequal(x, 0);
