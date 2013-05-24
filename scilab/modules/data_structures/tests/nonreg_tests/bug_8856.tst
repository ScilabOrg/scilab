// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 8856 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8856
//
// <-- Short Description -->
// [k,l,m,...]=find(a==5) works if a is a matrix (not an hypermat)

a = rand(2,2);
[i,j,k]=find(a>0.5);
assert_checkequal(and(k), %t);
