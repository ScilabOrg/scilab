// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10606 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10606
//
// <-- Short Description -->
//[rep,stat,stderr]=unix_g("diff /etc/passwd /etc/group")

//Shows only
// stderr  =
//     []
// stat  =
//    1.
// rep  =

[rep,stat,stderr]=unix_g("diff /etc/passwd /etc/group");
assert_checkfalse(rep == []);
