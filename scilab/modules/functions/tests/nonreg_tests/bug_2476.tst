// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2476
//
// <-- Short Description -->
//    Problems with quit and exit when using fun2string


function foo1,exit,endfunction
function foo2,quit,endfunction

if execstr('fun2string(foo1)','errcatch')<>0 then pause,end
if execstr('fun2string(foo2)','errcatch')<>0 then pause,end
