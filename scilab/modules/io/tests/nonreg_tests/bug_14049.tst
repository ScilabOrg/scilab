//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14049 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14049
//
// <-- Short Description -->
// genlib  hangs if an unexpected endfunction occurs

mkdir(TMPDIR + "bug_14049");
fun = [ "function r=foo()"
"x=1"
"endfunction"
"endfunction"];

mputl(fun, TMPDIR + "bug_14049"+filesep()+"bug_14049.sce");
genlib("bug_14049lib",TMPDIR+"bug_14049",%t);
