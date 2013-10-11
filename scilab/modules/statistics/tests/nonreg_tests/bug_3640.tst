// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3640
//
// <-- Short Description -->
// Calculating the mean value in the 3rd dimension "mean(A,3)", gives an error for input matrices of a certain size


test = rand(150,1,165);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [150 1]);

test = rand(150,1,166);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [150 1]);

test = rand(1000,1,165);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [1000 1]);

test = rand(1000,1,166);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [1000 1]);

test = rand(250,1,165);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [250 1]);

test = rand(250,1,166);
assert_checkfalse(execstr("r = mean(test,3);"   ,"errcatch") <> 0);
assert_checkequal(size(r), [250 1]);
