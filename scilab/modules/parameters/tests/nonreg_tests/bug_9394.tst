// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9394 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9394
//
// <-- Short Description -->
// is_params() recognized the "plist" field.

p = init_param();
bool  = is_param(p, "plist");
assert_checkequal(bool, %f);
p = add_param(p, "field1", 10);
bool  = is_param(p, "plist");
bool2 = is_param(p, "field1");
assert_checkequal(bool, %f);
assert_checkequal(bool2, %t);
