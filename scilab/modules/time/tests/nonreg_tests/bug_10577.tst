// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10577 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10577
//
// <-- Short Description -->
//
// getdate did not check input parameter.
//

msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: A real expected.\n"),"getdate", 1);
assert_checkerror("getdate(%i)", msgerr);