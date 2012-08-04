// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 11618 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11618
//
// <-- Short Description -->
//    if A is a complex sparse matrix, A(:) returned wrong values

A = [0 %i;1 2]
A1 = sparse(A);
assert_checkequal(full(A1(:)), A(:));

A = [1 0; 3 %i];
A1 = sparse(A);
assert_checkequal(full(A1(:)), A(:));

A = [%i 3; 0 1];
A1 = sparse(A);
assert_checkequal(full(A1(:)), A(:));

A = [1 %i;%i 0];
A1 = sparse(A);
assert_checkequal(full(A1(:)), A(:));
