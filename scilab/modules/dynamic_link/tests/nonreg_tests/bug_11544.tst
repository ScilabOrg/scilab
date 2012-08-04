// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 10544 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11544
//
// <-- Short Description -->
// genrated cleaner, try to remove "Debug" and "Release" folders.

ilib_verbose(0);

mkdir(TMPDIR + "/bug_11544");
cd(TMPDIR + "/bug_11544");

src = [
   '#include ""api_scilab.h""'
   'int sci_bug_11544(char* fname, int len)' 
   '{'
   '    AssignOutputVariable(pvApiCtx, 0) = 1;'
   '    return 0;'
   '}'];

mputl(src, "bug_11544.c');


//Release mode
setenv("DEBUG_SCILAB_DYNAMIC_LINK", "NO");
ilib_build('gw_bug_11544', ['bug_11544', 'sci_bug_11544'], 'bug_11544.c', []);

exec loader.sce;
bug_11544();
ulink();

assert_checktrue(or(ls() == "Release"));
exec cleaner.sce;
assert_checkfalse(or(ls() == "Release"));

//Debug mode
setenv("DEBUG_SCILAB_DYNAMIC_LINK", "YES");
ilib_build('gw_bug_11544', ['bug_11544', 'sci_bug_11544'], 'bug_11544.c', []);

exec loader.sce;
bug_11544();
ulink();

assert_checktrue(or(ls() == "Debug"));
exec cleaner.sce;
assert_checkfalse(or(ls() == "Debug"));
