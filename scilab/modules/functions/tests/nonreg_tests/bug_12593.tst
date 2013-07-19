// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12593 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12593
//
// <-- Short Description -->
// Unrelated error message when running genlib on a sci file containing a scilab function
// with a missing parenthesis
// Error: "mprintf: wrong number of input arguments: data do not fit with format"

my_function = ["function test()";"foo(";"endfunction"];
my_dir      = pathconvert(TMPDIR+"/bug12593_dir");
my_file     = my_dir+"test.sci";

mkdir(my_dir);
mputl(my_function,my_file);

errmsg=msprintf(gettext("%s: Error in file %s : %s.\n"), "genlib", my_file, msprintf(gettext("Invalid factor.\n")));
assert_checkerror("genlib(""bug_12593_dir"", my_dir,%T)",errmsg);

