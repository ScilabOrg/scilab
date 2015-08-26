// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal([1, -1], [1 -1]);
assert_checkequal(0, [1 - 1]);
a = 42;
assert_checkequal([1, -a], [1 -a]);
assert_checkequal([(1 - a)], [1 - a]);
assert_checkequal([1, - a + 2], [1 -(a - 2)]);
assert_checkequal([(1 - a + 2)], [1 - a + 2]);

M = matrix(1:4, 2, 2); B = [2; 4];
C = M^-1*B;
assert_checkequal(C, [2;0]);

