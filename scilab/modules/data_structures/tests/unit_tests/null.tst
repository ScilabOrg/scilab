// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
A=list(rand(23,47),string(rand(22,43)),list(sprand(11,23,1),"foo"));
assert_checkequal(A, A(null()));
