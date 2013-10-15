// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

assert_checkequal(bitshift(2^10,  1), 2^11);
assert_checkequal(bitshift(2^10, -1), 2^9);
