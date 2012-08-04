// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2553 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bugAdmin_II/show_bug.cgi?id=2553
//
// <-- Short Description -->
//   error recovery problem

function toto();x=1;y=x(3)+1;endfunction
function titi();execstr('toto();disp ok');endfunction
ierr=exec(titi,'errcatch',-1)		
		
if ierr<>21 then pause,end
