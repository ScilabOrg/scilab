// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - DIGITEO - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for archiveList function
// ============================================================================

files = archiveList('SCI/modules/fileio/tests/unit_tests/example.tar');
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),'example1.txt');
assert_checkequal(files(2),'example2.txt');

files = archiveList("SCI/modules/fileio/tests/unit_tests/example.tar.gz");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),'example1.txt');
assert_checkequal(files(2),'example2.txt');

files = archiveList("SCI/modules/fileio/tests/unit_tests\example.tar.gz");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),'example1.txt');
assert_checkequal(files(2),'example2.txt');

files = archiveList("SCI/modules/fileio/tests\unit_tests/example.tar.gz");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),'example1.txt');
assert_checkequal(files(2),'example2.txt');

ierr = execstr('archiveList(SCI)','errcatch');
assert_checkequal(ierr,999);

ierr = execstr('archiveList(SCI,1)','errcatch');
assert_checkequal(ierr,77);

