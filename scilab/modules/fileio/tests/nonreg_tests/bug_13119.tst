// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13119 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13119
//
// <-- Short Description -->
// mget and mgetl did not accept integer type as number of lines

curdir = pwd();
cd(TMPDIR);

fd = mopen("test", "wr"); // Open a file

res = mget(int8(1), "d", fd);
assert_checkequal(res, []);

res = mgetl("test", int8(1));
assert_checkequal(res, []);

mclose(fd);
deletefile("test");

cd(curdir);
