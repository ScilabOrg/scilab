// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6744 -->
// <-- CLI SHELL MODE -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6744
//
// <-- Short Description -->
//  p_margin() returns an erroneous result
z=poly(0,'z');
num=(0.0373327-0.0405775*z-0.1413155*z^2+0.1783403*z^3);
den=(-0.0373327+0.0405775*z+0.1413155*z^2-0.1783403*z^3+0.04867*z^4-0.04007*z^5-0.17482*z^6+0.2*z^7);
H=syslin(1.0,num/den);
[m,fr]=p_margin(H);


r=repfreq(H,fr);
//check if fr give a response with modulus=1
if abs(abs(r)-1)>1d-10 then pause,end
if abs(m-58.03)>0.01 then pause,end
if abs(m-pmodulo(atand(imag(r),real(r))-180,360))>1d-10 then pause,end
