// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7619 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7619
//
// <-- Short Description -->
// get_function_path did not return all available paths.
//

cd TMPDIR;
mkdir("bug_7619");
cd("bug_7619");
mputl("function r = factorial(), endfunction", TMPDIR + "/bug_7619/factorial.sci");
genlib("bug_7916lib", TMPDIR + "/bug_7619", %f ,%f);
bug_7916lib = lib(TMPDIR + "/bug_7619");
p = get_function_path("factorial");
if size(p, "*") <> 2 then pause, end;
