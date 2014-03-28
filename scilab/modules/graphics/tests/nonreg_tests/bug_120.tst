// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 120 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=120
//
// <-- Short Description -->
//  It seems that function bode doesn't work properly.
//  It shift x (horizontal) axis for one decade in right. So the bode plot is incorect.

// create a linerar system
s=poly(0,"s")
h=syslin("c",(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
my_title="(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)";

// draw the bode plot between fmin and fmax
fmin = 0.01;
fmax = 100;
bode(h,fmin,fmax,my_title);

// check that the axes data bounds are fmin and fmax
curFig = gcf();

// get data bounds of phase plot
dataBounds1 = curFig.children(1).data_bounds
// check that the X bounds are [fmin, fmax]
if (dataBounds1(1,1) <> fmin) then pause; end;
if (dataBounds1(2,1) <> fmax) then pause; end;

// same for magnitude plot
dataBounds2 = curFig.children(2).data_bounds
// check that the X bounds are [fmin, fmax]
if (dataBounds2(1,1) <> fmin) then pause; end;
if (dataBounds2(2,1) <> fmax) then pause; end;


