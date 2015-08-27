//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 13829 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13829
//
// <-- Short Description -->
// mean and sum functions returned wrong results for hypermatrices.

a = matrix(1:24, 1:4);
s = sum(a,4);
ref = matrix([40  44  48  52  56  60], 1:3);
assert_checkequal(s, ref);

a = ones(1,2,3,4);
m = mean(a,4);
ref = ones(1,2,3,1);
assert_checkequal(m, ref);

// Bug #10059 (duplicate)
a = matrix(1:12, [4, 1, 3]);
assert_checkequal(sum(a, 3), [15;18;21;24]);

a = matrix(1:24, [4, 2, 3]);
assert_checkequal(sum(a, 3), [27 39;30 42;33 45;36 48]);