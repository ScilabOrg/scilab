// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12679 -->
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12679
//
// <-- Short Description -->
// Checks on gcd() and lcm() arguments

// Run with test_run('polynomials', 'bug_12679', ['no_check_error_output'])

////////////////// gcd
// Normal behavior, with polynomials
s = poly(0, 's');
p = [s, s*(s+1)^2, 2*s^2+s^3];
[pgcd, u] = gcd(p);
assert_checkequal(p*u, [0 0 s]);
// Complex polynomials should yield an error
s = poly(%i, 's');
p = [s, s*(s+1)^2,2*s^2+s^3];
refMsg = msprintf(_("%s: Wrong type for argument #%d: Real Polynomial expected.\n"), "gcd", 1);
assert_checkerror("[pgcd, u] = gcd(p);", refMsg);

// Normal behavior, with integers
V = int32([2^2*3^5, 2^3*3^2, 2^2*3^4*5]);
[thegcd, U] = gcd(V);
assert_checkequal(V*U, int32([0 0 36]));
assert_checkequal(gcd(uint8([15 20])), uint8(5));
assert_checkequal(gcd([iconvert(15, 4) iconvert(20, 4)]), int32(5));
assert_checkequal(gcd(iconvert([15 20], 4)), int32(5));
// Trying to use doubles should yield an error
refMsg2 = msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "gcd", 1);
assert_checkerror("gcd([15 20]);", refMsg2);


////////////////// lcm
// Normal behavior, with polynomials
s = poly(0, 's');
p = [s, s*(s+1)^2, s^2*(s+2)];
[pp, fact] = lcm(p);
assert_checkequal(pp, [2*s^2 + 5*s^3 + 4*s^4 + s^5]);
// Complex polynomials should yield an error
s = poly(%i, 's');
p = [s, s*(s+1)^2, s^2*(s+2)];
refMsg3 = msprintf(_("%s: Wrong type for argument #%d: Real Polynomial expected.\n"), "lcm", 1);
assert_checkerror("[pp, fact] = lcm(p);", refMsg3);

// Normal behavior, with integers
V = int32([2^2*3^5, 2^3*3^2, 2^2*3^4*5]);
assert_checkequal(lcm(V), int32(9720));
// Trying to use doubles should yield an error
refMsg4 = msprintf(_("%s: Wrong type for argument #%d: Integer array or Polynomial expected.\n"), "lcm", 1);
assert_checkerror("lcm([15 20]);", refMsg4);
