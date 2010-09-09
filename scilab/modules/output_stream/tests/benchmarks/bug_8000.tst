// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- BENCH NB RUN : 1000 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8000
//

ilib_verbose(0);

test_path = SCI+'/modules/output_stream/tests/benchmarks/bug_8000.tst';

currentpath = pwd();

cd TMPDIR;
cd ../;
OS_TMP_DIR = pwd();


mkdir(OS_TMP_DIR, 'bug_8000');
TEST_DIR = OS_TMP_DIR + filesep() + 'bug_8000';

copyfile(SCI+'/modules/output_stream/tests/benchmarks/bug_8000.c' , TEST_DIR + filesep() + 'bug_8000.c');

chdir(TEST_DIR);

files=['bug_8000.c'];
ilib_build('libbug_8000',['benchsciprint','sci_benchsciprint'],files,[]);

// load the shared library 
exec loader.sce;

chdir(currentpath);


// <-- BENCH START -->
benchsciprint()
// <-- BENCH END -->

// ulink() all libraries
ulink();

//remove TMP_DIR
rmdir(TEST_DIR,'s');
// =============================================================================
