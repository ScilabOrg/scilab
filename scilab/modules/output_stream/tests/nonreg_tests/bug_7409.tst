// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Artem Glebov
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 7409 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7409
//
// <-- Short Description -->
// readNamedMatrixOfDouble (readCommonNamedMatrixOfDouble) returned wrong values

ilib_verbose(0);
test_path = get_absolute_file_path('bug_7409.tst');
currentpath = pwd();
cd TMPDIR;
cd ../;
OS_TMP_DIR = pwd();
mkdir(OS_TMP_DIR, 'bug_7409');
TEST_DIR = OS_TMP_DIR + filesep() + 'bug_7409';
copyfile(SCI + '/modules/output_stream/tests/nonreg_tests/bug_7409.c' , TEST_DIR + filesep() + 'bug_7409.c');
chdir(TEST_DIR);
files = ['bug_7409.c'];
ilib_build('lib_bug_7409', ['empty_string_test', 'sci_empty_string_test'], files, []);
exec('loader.sce');
chdir(currentpath);
if execstr("empty_string_test()", "errcatch") <> 0 then pause, end
