// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7118 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7118
//
// <-- Short Description -->
//    "bode" plot produces incorrect Magnitude response.


// <-- INTERACTIVE TEST -->
s = poly(0,'s')
G = syslin('c',1/s)
bode(G,0.01,100)
// The 0DB crossing should occur at 1Hz and the magnitude (Db) at freq = 0.001
// should be 40 DB and the magnitude at 100Hz should be -40 DB.

// Other test cases
s = poly(0,'s')
G = syslin('c',1/s)
clf, bode(G,0.01,100)

// 2) (freq, repf) 
nu=logspace(-2,2,100);
h=horner(1/%s,%i*nu);
clf, bode(nu,h)

// 3) (freq, db, phi)
db=log10(abs(h))*20;
phi=atand(imag(h),real(h));
clf, bode(nu,db,phi)
