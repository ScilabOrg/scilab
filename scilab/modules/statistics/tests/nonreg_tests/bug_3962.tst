// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3962 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3962
//
// <-- Short Description -->
// First lhs argument of pca no more contains a second column.

a=rand(100,10,'n');
[lambda,facpr,comprinc] = pca(a);
if size(lambda,2)<>2 then pause,end

