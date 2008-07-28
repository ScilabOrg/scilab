// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3260 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3260
//
// <-- Short Description -->
// X1=1;X2=7;x1=int8(X1);x2=int8(X2); x1\x2
// scilab 4.1.2 --> 7
// scilab trunk rev 25935 --> 4

X1=1;
X2=7;
x1=int8(X1);
x2=int8(X2);

if (x1\x2) <> 7 then pause,end

