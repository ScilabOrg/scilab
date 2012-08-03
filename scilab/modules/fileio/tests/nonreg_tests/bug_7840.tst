// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7840 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7840
//
// <-- Short Description -->
//  big lines were splitted by mgetl.
//

lines_res = mgetl("SCI/modules/fileio/tests/nonreg_tests/bug_7840.txt");
r = size(lines_res);
ref = [1,1];
if or(r <> ref) then pause, end
if length(lines_res) <> 10604 then pause, end

