// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4319 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4319
//
// <-- Short Description -->
//  The function nthresh called by the function parrt in ccontrg.sci is
//  missing
//2 states
A = [-5,-6;
     6,-4]
//five inputs
B = [0,-14,17,2,9;
     2,17,19,-10,-9]
//three outputs
C = [0,-17;
     3,-14;
     8,7]

D = [1,1,1,3,4;
     1,1,6,0,2;
     1,1,7,5,5]
PP=syslin('c',A,B,C,D);
gopt=gamitg(PP,[1 1]);

if execstr('KK=ccontrg(PP,[1 1],gopt+0.1)','errcatch')<>0 then pause,end
