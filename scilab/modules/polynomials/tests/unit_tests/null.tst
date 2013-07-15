// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
A=rand(23,47)+rand(23,47)*%s+rand(23,47)*%s^2+rand(23,47)*%s^3;
assert_checkequal(A, A(null()));