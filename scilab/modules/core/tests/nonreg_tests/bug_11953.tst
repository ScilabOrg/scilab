// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11953 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11953
//
// <-- Short Description -->
// Scilab crashes when global("") is entered.

errmsg = msprintf(gettext("%s : Wrong value for argument #%d: %s\n"), "global", 1, "");
assert_checkerror("global("""")",errmsg);
errmsg = msprintf(gettext("%s : Wrong value for argument #%d: %s\n"), "global", 2, "");
assert_checkerror("global(""a"","""")",errmsg);
errmsg = msprintf(gettext("%s : Wrong value for argument #%d: %s\n"), "global", 3, "");
assert_checkerror("global(""a"",""b"","""")",errmsg);

l=list("a", "b","c", "");
errmsg = msprintf(gettext("%s : Wrong value for argument #%d: %s\n"), "global", 4, "");
assert_checkerror("global(l(:))",errmsg);
