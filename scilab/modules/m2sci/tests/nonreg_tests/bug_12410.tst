// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014      - Scilab-Enterprises - Pierre-Aime Agnel <pierre-aime.agnel@scilab-enterprises.com>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12410 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12410
//
// <-- Short Description -->
// Crash when using matfile2sci on a complex file

ierr = execstr("matfile2sci(""SCI/modules/m2sci/tests/nonreg_tests/bug_12410.mat"", ""TMPDIR/bug_12410.bin"");", "errcatch");
if ierr<>0 then pause;end
ierr = execstr("load(""TMPDIR/bug_12410.bin"");", "errcatch");
if ierr<>0 then pause;end
