// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13217 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13217
//
// <-- Short Description -->
// augment() yielded a wrong result when flag2="i".

// We check that the last element of P is [G -G], as shown in the help page.
G = 1/%s;

flag = "S";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "SR";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "ST";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "SRT";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "R";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "RT";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);

flag = "T";
[P, r] = augment(G, flag, "i");
assert_checkequal(P($, :), [G -G]);
