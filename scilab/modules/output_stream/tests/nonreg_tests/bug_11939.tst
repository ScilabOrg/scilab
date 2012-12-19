// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 11939 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11939
//
// <-- Short Description -->
// ssprint formatting was slightly not right.

a = [1 1;0 1];
b = [0 1;1 0];
c = [1, 1];
d = [3, 2];
modelC = syslin("c", a, b, c, d);
modelD = syslin("d", a, b, c, d);
// Test display (do not add semi-colon for lines below)
ssprint(modelC)
ssprint(modelD)