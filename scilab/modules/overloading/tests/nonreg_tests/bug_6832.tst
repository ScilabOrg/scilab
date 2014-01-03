// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 6832 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6832
//
// <-- Short Description -->
// Matrices of rationals can now be transposed via the " .' " operator.

num = (%z-%i).^grand(2, 4, "uin", 0, 2);
den = (%z+%i).^grand(2, 4, "uin", 0, 2);
R = num ./ den;
refR = ones(4, 2);

// Manually transposing R
for i=1:4
    refR(i, 1) = R(1, i);
    refR(i, 2) = R(2, i);
end

assert_checkequal(R.', refR);
