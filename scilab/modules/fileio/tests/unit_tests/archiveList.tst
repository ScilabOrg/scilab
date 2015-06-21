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
if length(length(files)) <> 2 | files(1) <> 'example1.txt' | files(2) <> 'example2.txt' then pause,end

files = archiveList("SCI/modules/fileio/tests/unit_tests/example.tar.gz");
if length(length(files)) <> 2 | files(1) <> 'example1.txt' | files(2) <> 'example2.txt' then pause,end

files = archiveList("SCI/modules/fileio/tests/unit_tests\example.tar.gz");
if length(length(files)) <> 2 | files(1) <> 'example1.txt' | files(2) <> 'example2.txt' then pause,end

files = archiveList("SCI/modules/fileio/tests\unit_tests/example.tar.gz");
if length(length(files)) <> 2 | files(1) <> 'example1.txt' | files(2) <> 'example2.txt' then pause,end

files = archiveList('SCI/modules/fileio/tests/unit_tests/dummy');

ierr = execstr('archiveList(SCI)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('archiveList(SCI,1)','errcatch');
if ierr <> 77 then pause,end
