// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 7836 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7836
//
// <-- Short Description -->
// The sub_tics property does not correctly track the number of
// sub-ticks when switching scale.
//

x = logspace(-1,1,100);
y = 1 ./ x;
scf();  
plot(x,y);
a = gca();

formerSubtics = a.sub_tics;

a.log_flags = 'lln'; // switch to log scale

// sub_tics should have changed due to the switch to log scale.
if (a.sub_tics(1) == formerSubtics(1) | a.sub_tics(2) == formerSubtics(2)) then pause; end;

