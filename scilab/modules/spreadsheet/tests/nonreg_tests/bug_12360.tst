// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12360 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12360
//
// <-- Short Description -->
// Imaginary part of purely real complex is printed in CSV

realMatrix = [0.211, 3, -4.0, 0.0];
purelyRealComplexMatrix = complex(realMatrix);

filename = fullfile(TMPDIR, 'bug_12360.csv');
write_csv(purelyRealComplexMatrix, filename, ascii(9), ".", "%5.3f");

path = SCI + "/modules/spreadsheet/tests/nonreg_tests";
assert_checkfilesequal(filename, fullfile(path, "bug_12360.ref.csv"));