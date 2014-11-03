// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - SCilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

res(2,3,4) = 0;
computed = list([], 1, 10, list(2,3), list(2,3,4));
expected = list([], 0, 0, [0 0 0;0 0 0], res);

for i = 1:size(computed)
    assert_checkequal(zeros(computed(i)(:)), expected(i));
end
