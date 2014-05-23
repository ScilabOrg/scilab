// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7958 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7958
//
// <-- Short Description -->
// mrfit fails when there are four input arguments.

w=0.01:0.01:2;
s=poly(0,'s');
G=syslin('c',2*(s^2+0.1*s+2),(s^2+s+1)*(s^2+0.3*s+1)); // syslin('c',Num,Den);
fresp=repfreq(G,w);
mag=abs(fresp);

assert_checktrue(execstr("mrfit(w,mag,4,2*ones(length(w),1))","errcatch")==0);
G = mrfit(w,mag,4,ones(w)');
G_ref = mrfit(w,mag,4);
assert_checkequal(G, G_ref);
