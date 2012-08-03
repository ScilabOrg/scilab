// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10641 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10641
//
// <-- Short Description -->
// string(:) gave wrong results
//

a = : ;
assert_checkequal(string(a), "");
assert_checkequal(isdigit(string(a)), []);

assert_checkequal("h" + string(a) + "hop", "hhop");

msgerr = msprintf(gettext("Eye variable undefined in this context.\n"));
assert_checkerror("a(string(a))" , msgerr);
assert_checkerror("a(:)" , msgerr);
assert_checkequal(size(string(a)), [1 1]);
assert_checkequal(size(string(:)), [1 1]);