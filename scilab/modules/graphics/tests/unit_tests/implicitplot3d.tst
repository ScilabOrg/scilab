// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Unit test for implicitplot3d  -->
//
// <-- TEST WITH GRAPHIC -->

// Error testing
err_msg = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 2);
text_fun = "implicitplot3d(""this does not work"", ""test_x"", ""test_y"", ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 2);
text_fun = "implicitplot3d(""this does not work"", [1 2; 3 4], ""test_y"", ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 2);
text_fun = "implicitplot3d(""this does not work"", [%i %i], ""test_y"", ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 3);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, ""test_y"", ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 3);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, [1 2; 3 4], ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 3);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, [%i, %i], ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong type for input argument #%d: Real vector expected.\n"), "implicitplot3d", 4);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, -1:0.1:1, ""test_z"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong size for input argument #%d: Real vector expected.\n"), "implicitplot3d", 4);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, -1:0.1:1, [1 2; 3 4])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong value for input argument #%d: Real value expected.\n"), "implicitplot3d", 4);
text_fun = "implicitplot3d(""this does not work"", -1:0.1:1, -1:0.1:1, [1 2 %i])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong type for input argument #%d: Real hypermatrix or String or Scilab function expected.\n"), "implicitplot3d", 1);
text_fun = "implicitplot3d([%s, %s, 2 + %s], -1:0.1:1, -1:0.1:1, -1:0.1:1)";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong type for input argument #%d: Real hypermatrix expected.\n"), "implicitplot3d", 1);
text_fun = "implicitplot3d(repmat([""a"", ""b""], 2, 2, 2), -1:0.1:1, -1:0.1:1, -1:0.1:1)";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong value for input argument #%d: Real hypermatrix expected.\n"), "implicitplot3d", 1);
text_fun = "implicitplot3d(repmat(%i, 2, 2, 2), -1:0.1:1, -1:0.1:1, -1:0.1:1)";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d-by-%d matrix expected.\n"), "implicitplot3d", 1, 21, 21, 21);
text_fun = "implicitplot3d(zeros(2, 2, 2), -1:0.1:1, -1:0.1:1, -1:0.1:1)";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(gettext("%s: Wrong size for input argument #%d: Scalar string expected.\n"), "implicitplot3d", 1);
text_fun = "implicitplot3d([""a"", ""b""], -1:0.1:1, -1:0.1:1, -1:0.1:1)";
assert_checkerror(text_fun, err_msg);

// Nominal behaviour
implicitplot3d("x^2 + y^2 + z^2 - 1");
ent = gce();

calc = (ent.data.x).^2 + (ent.data.y).^2 + (ent.data.z).^2 - 1;
errQuad = norm(calc) / size(calc, '*');

// Relative error is under 1%
assert_checktrue(errQuad < 0.01);
