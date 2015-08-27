//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2127 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2127
//
// <-- Short Description -->
//    Rank-increasing extraction can fail for booleans

T=%t([1 1],[1 1],[1 1]) ;
if or(T(:,:,1)<>[%t %t;%t %t]) then pause,end
if or(T(:,:,2)<>[%t %t;%t %t]) then pause,end

T=%s([1 1],[1 1],[1 1]) ;
if or(T(:,:,1)<>[%s %s;%s %s]) then pause,end
if or(T(:,:,2)<>[%s %s;%s %s]) then pause,end

t='x';
T=t([1 1],[1 1],[1 1]) ;
if or(T(:,:,1)<>[t t;t t]) then pause,end
if or(T(:,:,2)<>[t t;t t]) then pause,end
