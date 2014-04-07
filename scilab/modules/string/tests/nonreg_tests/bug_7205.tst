// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7205 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7205
//
// <-- Short Description -->
// length() applied to a non string hypermatrix returns 3 instead size(H,"*")
//

A=rand(2,2,2,2);
assert_checkequal(length(A),double(prod(A.dims)));
A=rand(5,5,5,5,5,5,5);
assert_checkequal(length(A),double(prod(A.dims)));
