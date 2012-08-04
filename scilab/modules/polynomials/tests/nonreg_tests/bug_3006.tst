// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3006 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3006
//
// <-- Short Description -->
// submatrix assignment impossible when the matrix is indexed with a boolean array of falses  
s=poly(0,'s');
a=[1+s 2;3 4*s];a_ref=a;

a(1,[%f %f])=[];
if or(a<>a_ref) then pause,end
a([%f %f],1)=[];
if or(a<>a_ref) then pause,end
a([%f %f],[%f %f])=[];
if or(a<>a_ref) then pause,end

a(1,$:1)=[];
if or(a<>a_ref) then pause,end
a($:1,1)=[];
if or(a<>a_ref) then pause,end
a($:1,$:1)=[];
if or(a<>a_ref) then pause,end
