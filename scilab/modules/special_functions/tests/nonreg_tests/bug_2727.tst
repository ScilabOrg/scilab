// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2727 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2727
//

t = linspace(0.2,10,60);
X = t'*ones(t);
Y = ones(t')*t;
ierr = execstr("Z = beta(X,Y);","errcatch");
if ierr <> 0 then pause,end;

