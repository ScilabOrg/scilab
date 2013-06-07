// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 12657 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12657
//
// <-- Short Description -->
// Computation of v1.^v2 was not done in place,
// when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

// <-- ENGLISH IMPOSED -->

// Run with test_run('double', 'bug_12657', ['no_check_error_output'])

// Checking that the results for said cases are still ok
assert_checkequal([0.5 2; 100000 4.5578].^2, [0.25 4; 1e10 20.7735408400000025608]);
assert_checkequal([2.5].^[2 5; 10 5], [6.25 97.65625; 9536.7431640625 97.65625]);
assert_checkequal([0.5 2; 100000 4.5578].^[2 5; 5 2], [0.25 32; 1e25 20.7735408400000025608]);

// Now, checking that computation is done in place
stacksize(2.5e6);
v = rand(1, 1e6);
v.^2;
