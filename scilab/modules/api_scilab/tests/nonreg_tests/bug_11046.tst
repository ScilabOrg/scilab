// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11046 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11046
//
// <-- Short Description -->
// Sometimes, OverLoad(0) did not call %_xxx but %x_xxx overload

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/bug_11046"));
cd(pathconvert(TMPDIR+"/bug_11046"));
copyfile(SCI+"/modules/api_scilab/tests/nonreg_tests/bug_11046.c",pathconvert(TMPDIR+"/bug_11046/bug_11046.c",%F));
ilib_build("gw_bug_11046", ["bug_11046", "sci_bug_11046"], "bug_11046.c", []);
exec("loader.sce");

function %_bug_11046()
    disp('OK default overload called.')
endfunction

//call %_myscifun
bug_11046();


function [x] = call_overload(a, b)
    bug_11046();
    x = 2;
endfunction

function call_overload2(a, b)
endfunction

//try to call %s_bug_11046
call_overload2(1, call_overload(1,2));
