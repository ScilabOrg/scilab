// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 8561 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8561
//
// <-- Short Description -->
//    Absolute and relative tolerances sizes were not properly checked.

// Run with test_run('differential_equations', 'bug_8561', ['no_check_error_output'])

m = 1;
l = 1;
g = 10;
theta0 = 0.2;
fi0 = %pi/4;
x0  = l*sin(fi0)*cos(theta0);
y0  = l*cos(fi0)*cos(theta0);
z0  = l*sin(theta0);

Y0  = [x0; y0; z0; cos(fi0)/sin(fi0); -1; 0; 0];
Yd0 = [Y0(4:6) ; 0; 0; -g; 0];

t = 0:0.01:15;
function [res, ires] = f(t, y, yd)
  res = [yd(1:3)-y(4:6);
         yd(4:6)+2*y(1:3)*y(7)/m+[0; 0; g];
         2*y(4:6)'*y(1:3)];
  ires = 0;
endfunction

//rtol and atol should have 7 elements, because neq = 7
rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4];
atol = 2*rtol;

assert_checkfalse(execstr("y = dassl([Y0 Yd0], 0, t, atol, rtol, f);", "errcatch") == 0);
refMsg = msprintf(_("Wrong size for argument: Incompatible dimensions.\n"));
assert_checkerror("y = dassl([Y0 Yd0], 0, t, atol, rtol, f);", refMsg);

rtol = [1e-5, 1e-5, 1e-5, 1e-4, 1e-4, 1e-4, 1e-3];
atol = 2*rtol;
assert_checktrue(execstr("y = dassl([Y0 Yd0], 0, t, atol, rtol, f);", "errcatch") == 0);

