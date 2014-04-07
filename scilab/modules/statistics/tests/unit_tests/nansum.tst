// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for nansum function
// =============================================================================

assert_checkfalse(execstr("nansum()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "nansum", 1, 2);
assert_checkerror("nansum()", refMsg);

assert_checkfalse(execstr("nansum(""s"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real or complex, boolean, polynomial matrix expected.\n"), "nansum", 1);
assert_checkerror("nansum(""s"")", refMsg);

assert_checkfalse(execstr("nansum(1, ""p"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "nansum", 2, """r"", ""c"", ""*"", 1, 2");
assert_checkerror("nansum(1, ""p"")", refMsg);

assert_checkfalse(execstr("nansum(1, %s)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"), "nansum", 2);
assert_checkerror("nansum(1, %s)", refMsg);

assert_checkequal(nansum([]), 0);
assert_checkequal(nansum(%nan), 0);
assert_checkequal(nansum([%nan %nan; 1 1]), 2);
assert_checkequal(nansum([%nan %nan; 1 1], "r"), [1 1]);
assert_checkequal(nansum([%nan %nan; 1 1], "c"), [0; 2]);
assert_checkequal(nansum([%nan %nan; 1 1], 1), [1 1]);
assert_checkequal(nansum([%nan %nan; 1 1], 2), [0; 2]);

assert_checkequal(nansum([%nan 1; %nan 1]), 2);
assert_checkequal(nansum([%nan 1; %nan 1], "r"), [0 2]);
assert_checkequal(nansum([%nan 1; %nan 1], "c"), [1; 1]);
assert_checkequal(nansum([%nan 1; %nan 1], 1), [0 2]);
assert_checkequal(nansum([%nan 1; %nan 1], 2), [1; 1]);

assert_checkequal(nansum([1 3; %nan 6]), 10);
assert_checkequal(nansum([1 3; %nan 6], "r"), [1 9]);
assert_checkequal(nansum([1 3; %nan 6], "c"), [4; 6]);
assert_checkequal(nansum([1 3; %nan 6], 1), [1 9]);
assert_checkequal(nansum([1 3; %nan 6], 2), [4; 6]);

assert_checkequal(nansum([%nan %s; 1 1]), 2+%s);
assert_checkequal(nansum([%nan %s; 1 1], "r"), [1 1+%s]);
assert_checkequal(nansum([%nan %s; 1 1], "c"), [%s; 2]);
assert_checkequal(nansum([%nan %s; 1 1], 1), [1 1+%s]);
assert_checkequal(nansum([%nan %s; 1 1], 2), [%s; 2]);

assert_checkequal(nansum([%nan %t]), 1);
assert_checkequal(nansum([%nan %t], "r"), [0 1]);
assert_checkequal(nansum([%nan %t], "c"), 1);
assert_checkequal(nansum([%nan %t], 1), [0 1]);
assert_checkequal(nansum([%nan %t], 2), 1);
