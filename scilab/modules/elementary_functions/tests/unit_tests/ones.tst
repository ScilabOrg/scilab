// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - SCilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

res(2,3,4) = 1;
res(:) = 1;
computed = list([], 1, 10, list(2,3), list(2,3,4));
expected = list([], 1, 1, [1 1 1;1,1,1], res);

for i = 1:size(computed)
    assert_checkequal(ones(computed(i)(:)), expected(i));
end
