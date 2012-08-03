// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 7282 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7282
//
// <-- Short Description -->
// eval failed on some functions without output arguments.

str = "realtimeinit(60)";
r = eval(str);
if r <> [] then pause, end

function o = blabla()
 o = 3;
endfunction

r = eval("blabla()");
if r <> 3 then pause, end


