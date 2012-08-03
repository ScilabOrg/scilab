// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2277 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2277
//
// <-- Short Description -->
//lusolve produces erroneous results with Scilab5 (it was ok on Scilab4)

A=sparse([2,2;2,4;3,3;3,7;4,2;4,4;6,6;6,8;7,3;7,7;8,6;8,8],..
      [-1;1;-1;1;1;-1;-1;1;1;-1;1;-1],[10,10]) ;
[ptr,rkA]=lufact(A);
x=lusolve(ptr,zeros(10,1));ludel(ptr);
if norm(A*x)>1d-14 then pause,end


