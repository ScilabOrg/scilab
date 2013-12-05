// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10998 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10998
//
// <-- Short Description -->
// matrix*hypermatrix operation failed.

//---------------------------------------------------
// Matrix * Hypermatrix

clear
rand("seed", 0);
test1 = rand(3, 3, 3);
test2 = rand(3, 3, 3);

// Multiply a 3x3 matrix by a 3x1x3 hypermatrix.
c = test1(:, :, 1) * test2(:, 2, :);

// Build the expected result. Its size is 3x1x3.
refC = zeros(3, 1, 3);
for i=1:3
    refC(:, :, i) = test1(:, :, 1) * test2(:, 2, i);
end

assert_checkequal(c, refC);


// Now with harder dimensions
test1 = rand(5, 3);
test2 = rand(3, 4, 3);

// Multiply a 5x3 matrix by a 3x2x3 hypermatrix.
c = test1(:, :) * test2(:, 1:2, :);

// Build the expected result. Its size is 5x2x3.
refC = zeros(5, 2, 3);
for i=1:3
    refC(:, :, i) = test1(:, :) * test2(:, 1:2, i);
end

assert_checkequal(c, refC);


//---------------------------------------------------
// Hypermatrix * Matrix

test1 = rand(4, 3);
// Multiply a 3x4x3 matrix by a 4x3 hypermatrix.
c = test2(:, :, :) * test1(:, :);

// Build the expected result. Its size is 3x3x3.
refC = zeros(3, 3, 3);
for i=1:3
    refC(:, :, i) = test2(:, :, i) * test1(:, :);
end

assert_checkequal(c, refC);
