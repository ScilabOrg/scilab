// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4321 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4321
//
// Short description:
// [a,b]=(1,1); strcat(a,"x",b) Seg fault 
//
//==============================================================
[a,b]=(1,1); 
ierr = execstr("strcat(a,''x'',b)","errcatch");
if ierr <> 999 then pause,end
//==============================================================
