// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 8480 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8480
//
// <-- Short Description -->
// The debug block did not have a valid function type.
//

// start a clean scilab
Result_ref=[1,4,0;
1,4,0;
2,4,0;
2,4,0;
3,4,0;
3,4,0;
8,4,0;
8,4,0;
9,4,0;
9,4,0;
1,6,0;
1,6,0;
2,6,0;
2,6,0;
3,6,0;
3,6,0;
8,6,0;
8,6,0;
9,6,0;
9,6,0;
1,6,0;
1,6,0;
2,6,0;
2,6,0;
3,6,0;
3,6,0;
8,6,0;
8,6,0;
9,6,0;
9,6,0;
1,6,0;
1,6,0;
2,6,0;
2,6,0;
3,6,0;
3,6,0;
8,6,0;
8,6,0;
9,6,0;
9,6,0;
1,1,0;
1,1,0;
3,1,0;
3,1,0;
8,1,0;
8,1,0;
9,1,0;
9,1,0;
2,1,0;
2,1,0;
3,2,0;
3,2,0;
8,3,0;
8,3,0;
8,2,0;
8,2,0;
9,2,0;
9,2,0;
1,1,0.5;
1,1,0.5;
3,1,0.5;
3,1,0.5;
8,1,0.5;
8,1,0.5;
9,1,0.5;
9,1,0.5;
2,1,0.5;
2,1,0.5;
3,2,0.5;
3,2,0.5;
8,3,0.5;
8,3,0.5;
8,2,0.5;
8,2,0.5;
9,2,0.5;
9,2,0.5;
1,5,1;
1,5,1;
2,5,1;
2,5,1;
3,5,1;
3,5,1;
8,5,1;
8,5,1;
9,5,1;
9,5,1];
importXcosDiagram('SCI/modules/xcos/tests/nonreg_tests/bug_8480.xcos');
global Result;Result=[];
Info=scicos_simulate(scs_m,list());
if ~and(Result==Result_ref) then pause,end
//k=find(Result(:,1)<>Result_ref(:,1));
clearglobal Result
