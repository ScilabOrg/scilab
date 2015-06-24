// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for archiveExtract function
// ============================================================================

x = archiveExtract('SCI/modules/fileio/tests/unit_tests/example.tar');
if (~isfile("example1.txt") | ~isfile("example2.txt"))  then pause,end

deletefile("example1.txt");
deletefile("example2.txt");

x = archiveExtract('SCI/modules/fileio/tests/unit_tests/example.tar',%f);
if (~isfile("example1.txt") | ~isfile("example2.txt"))  then pause,end

deletefile("example1.txt");
deletefile("example2.txt");

x = archiveExtract('SCI/modules/fileio/tests/unit_tests/example.tar',%t,"archiveExtract");
if (~isfile("archiveExtract/example1.txt") | ~isfile("archiveExtract/example2.txt"))  then pause,end

removedir("archiveExtract");

ierr = execstr('archiveExtract(SCI)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('archiveExtract(SCI,1)','errcatch');
if ierr <> 999 then pause,end

