// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13919 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14082
//
// <-- Short Description -->
// hidden was a reserved keyword but it is not used.

hidden = 1;

function r = hidden(x,y)
    r = x + y;
endfunction

assert_checkequal(hidden(1,2), 3);

