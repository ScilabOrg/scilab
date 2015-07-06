// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4176 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4176
//
// <-- Short Description -->
// No way to draw polylines with too many points


stacksize("max")
Maximum=1e7
dt=1/Maximum;
t=0:dt:1;
y=sin(2*%pi*50*t);
plot(t,y);