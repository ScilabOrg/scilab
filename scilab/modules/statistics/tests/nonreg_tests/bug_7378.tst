// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7378 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7378
//
// <-- Short Description -->
// quart() did not process %nans correctly. Now, quart(x) returns %nan if x contains a %nan.
// This breaks backward compatibility but enables %nan propagation, rather than the %nans going undetected.
// =============================================================================

q1 = quart(%nan);
q2 = quart([%nan 1]);
q3 = quart([1; %nan]);

assert_checkequal(q1, %nan);
assert_checkequal(q2, %nan);
assert_checkequal(q3, %nan);
