// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2551 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bugAdmin_II/show_bug.cgi?id=2551
//
// <-- Short Description -->
//   error recovery problem

// Serge Steer - Scilab Project
// Copyright INRIA
// 6 feb 2008


function f();  x=1;y=x(3);endfunction
function g();exec(f);endfunction

ierr=exec(g,'errcatch',-1)		
if ierr<>21 then pause,end
