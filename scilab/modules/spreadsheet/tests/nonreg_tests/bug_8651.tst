// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 8651 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8651
//
// <-- Short Description -->
// The error message of write_csv was wrong.
//

a = ones(5,7);
filename = fullfile(TMPDIR, 'foo.txt');
msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), 'write_csv', 4, '.', ',');
assert_checkerror ("write_csv(a,filename, '";"', '"@"')" , msgerr);
