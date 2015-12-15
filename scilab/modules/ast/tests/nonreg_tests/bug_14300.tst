// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14300 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14300
//
// <-- Short Description -->
// Crash when playing with structures.

clear s;
s.a.h = 1;
s.b.h = 2;
s(:).h;
s(1)(:).h;
