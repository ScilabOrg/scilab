// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14028 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14028
//
// <-- Short Description -->
// force flag of genlib does not rebuild bin file.

cd TMPDIR;
mkdir("lib1");
mputl(["function ret = lib_test";"  ret= ""lib1""";"endfunction"], "lib1/lib_test.sci");

genlib("lib1lib","lib1",%f);
info1 = fileinfo("lib1/lib_test.bin");

genlib("lib1lib","lib1",%f);
info2 = fileinfo("lib1/lib_test.bin");
assert_checktrue(info1(6) == info2(6));

mputl(["function ret = lib_test";"  ret= ""lib1.2""";"endfunction"], "lib1/lib_test.sci");
genlib("lib1lib","lib1",%f);
info1 = fileinfo("lib1/lib_test.bin");

//to be sure date change
sleep(2000);

genlib("lib1lib","lib1",%t);
info2 = fileinfo("lib1/lib_test.bin");
assert_checkfalse(info1(6) == info2(6));
