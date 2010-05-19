// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6428 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6428
//
// <-- Short Description -->
// fscanfMat did not manage format parameter as fprintfMat

A = ones(5,5) + 0.1;
fprintfMat(TMPDIR+'/Matrix.txt',A,'%lg|blablabla|');
B = fscanfMat(TMPDIR+'/Matrix.txt','%lg|blablabla|');
if ~and(A == B) then pause,end;