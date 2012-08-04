// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10001 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10001
//
// <-- Short Description -->
// %inf:%inf crashes Scilab
assert_checkequal(1:0:1,[]);
assert_checkequal(1:0:2,[]);
assert_checkequal(2:0:1,[]);
assert_checkequal(%inf:0:%inf,[]);
assert_checkequal(-%inf:0:%inf,[]);
assert_checkequal(1:0:%inf,[]);
assert_checkequal(%inf:0:1,[]);

assert_checkequal(%nan:1:3,%nan);
assert_checkequal(1:%nan:3,%nan);
assert_checkequal(1:%nan,%nan);

assert_checkequal(%inf:%inf,%nan);
assert_checkequal(-%inf:1:%inf,%nan);

assert_checkequal(%inf:1,[]);
assert_checkequal(1:-%inf,[]);
assert_checkequal(%inf:-1:1,%nan);

