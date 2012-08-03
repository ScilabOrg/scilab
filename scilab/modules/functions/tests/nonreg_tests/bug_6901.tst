// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6901 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6901
//
// <-- Short Description -->
// This script with 'exec' produced a weird warning

mputl("exec("""", -1);", TMPDIR + "/bug.sce");
ierr = execstr("exec(TMPDIR + ''/bug.sce'');", "errcatch");
if ierr <> 241 then pause,end
