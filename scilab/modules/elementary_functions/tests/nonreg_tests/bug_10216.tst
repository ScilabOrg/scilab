// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10216 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10216
//
// <-- Short Description -->
// Invalid syntaxes for zeros, ones, eye, rand like zeros(2,:)

errmsg1=msprintf(_("Invalid index.\n"));
assert_checkerror("zeros(:)", errmsg1, 21);
assert_checkerror("zeros(:,2)", errmsg1, 21);
assert_checkerror("zeros(2,:)", errmsg1, 21);
assert_checkerror("zeros(:,:)", errmsg1, 21);

assert_checkerror("ones(:)", errmsg1, 21);
assert_checkerror("ones(:,2)", errmsg1, 21);
assert_checkerror("ones(2,:)", errmsg1, 21);
assert_checkerror("ones(:,:)", errmsg1, 21);

assert_checkerror("eye(:)", errmsg1, 21);
assert_checkerror("eye(:,2)", errmsg1, 21);
assert_checkerror("eye(2,:)", errmsg1, 21);
assert_checkerror("eye(:,:)", errmsg1, 21);

assert_checkerror("rand(:)", errmsg1, 21);
assert_checkerror("rand(:,""uniform"")", errmsg1, 21);
assert_checkerror("rand(:,""normal"")", errmsg1, 21);
assert_checkerror("rand(:,2)", errmsg1, 21);
assert_checkerror("rand(2,:)", errmsg1, 21);
assert_checkerror("rand(:,2,""uniform"")", errmsg1, 21);
assert_checkerror("rand(2,:,""uniform"")", errmsg1, 21);
assert_checkerror("rand(:,2,""normal"")", errmsg1, 21);
assert_checkerror("rand(2,:,""normal"")", errmsg1, 21);
