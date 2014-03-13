// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10942 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10492
//
// <-- Short Description -->
// soundsec does not sample at the given frequency and is one element too long
//

t=[0:1:0.001*8000-1]/8000;
assert_checkequal(soundsec(0.001,8000),t);
