// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12927 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12927
//
// <-- Short Description -->
// ones function could not take integer type input.

assert_checkequal(ones(4, -int32(5)), []);
assert_checkequal(ones(-4, int32(5)), []);
assert_checkequal(ones(-4, -int32(5)), []);
assert_checkequal(ones(int32(5), -4), []);
assert_checkequal(ones(-int32(5), 4), []);
assert_checkequal(ones(-int32(5), -4), []);
assert_checkequal(ones(-int32(5), -int32(4)), []);


// int8
x = int8(5);
res = ones(x, 4);
refRes = [
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1 ];

assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int8(5));

res = ones(4, x);
refRes = refRes';
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int8(5));

res = ones(x, x);
refRes(5, :) = [1 1 1 1 1];
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int8(5));

refMsg = msprintf(_("Wrong size for argument #%d.\n"), 1);
assert_checkerror("ones(int8([5 5]), 4)", refMsg);
refMsg = msprintf(_("Wrong size for argument #%d.\n"), 2);
assert_checkerror("ones(4, int8([5 5]))", refMsg);
assert_checkerror("ones(int8([5 5]), int8([5 5]))", refMsg);

// int16
x = int16(5);
res = ones(x, 4);
refRes = [
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1 ];

assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int16(5));

res = ones(4, x);
refRes = refRes';
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int16(5));

res = ones(x, x);
refRes(5, :) = [1 1 1 1 1];
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int16(5));

refMsg = msprintf(_("Wrong size for argument #%d.\n"), 1);
assert_checkerror("ones(int16([5 5]), 4)", refMsg);
refMsg = msprintf(_("Wrong size for argument #%d.\n"), 2);
assert_checkerror("ones(4, int16([5 5]))", refMsg);
assert_checkerror("ones(int16([5 5]), int16([5 5]))", refMsg);


// int32
x = int32(5);
res = ones(x, 4);
refRes = [
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1
1 1 1 1 ];

assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int32(5));

res = ones(4, x);
refRes = refRes';
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int32(5));

res = ones(x, x);
refRes(5, :) = [1 1 1 1 1];
assert_checkequal(size(res), size(refRes));
assert_checkequal(res, refRes);
assert_checkequal(x, int32(5));

refMsg = msprintf(_("Wrong size for argument #%d.\n"), 1);
assert_checkerror("ones(int32([5 5]), 4)", refMsg);
refMsg = msprintf(_("Wrong size for argument #%d.\n"), 2);
assert_checkerror("ones(4, int32([5 5]))", refMsg);
assert_checkerror("ones(int32([5 5]), int32([5 5]))", refMsg);
