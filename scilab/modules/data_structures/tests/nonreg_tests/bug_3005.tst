//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3005 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3005
//
// <-- Short Description -->
//can't modify a hypermatrix stored into a list.
a = list(33,ones(2,2,2)); 
if execstr('a(2)(2,2,2) = 3','errcatch')<>0 then pause,end
 
 
