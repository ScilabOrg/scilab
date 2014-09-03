// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Unit test for contour3d  -->
//
// <-- TEST WITH GRAPHIC -->

// Error testing

// x must be a real vector
err_msg = msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 1);
text_fun = "contour3d(""test_x"", ""test_y"", ""test_z"", ""test_f"", ""test_nf"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 2);
text_fun = "contour3d([0 1], ""test_y"", ""test_z"", ""test_f"", ""test_nf"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 3);
text_fun = "contour3d([0 1], [0 1], ""test_z"", ""test_f"", ""test_nf"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong type for input arguments #%d: Real vector expected"), "contour3d", 5);
text_fun = "contour3d([0 1], [0 1], [0 1], ""test_f"", ""test_nf"")";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 1);
text_fun = "contour3d([%i %i], [0 1], [0 1], ""test_f"", [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 2);
text_fun = "contour3d([0 1], [%i %i], [0 1], ""test_f"", [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 3);
text_fun = "contour3d([0 1], [0 1], [%i %i], ""test_f"", [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong value for input arguments #%d: Real matrix expected"), "contour3d", 4);
text_fun = "contour3d([0 1], [0 1], [0 1], [%i %i], [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong value for input arguments #%d: Real vector expected"), "contour3d", 5);
text_fun = "contour3d([0 1], [0 1], [0 1], [0 1], [%i %i])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 1);
text_fun = "contour3d(1, [0 1], [0 1], [0 1], [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 2);
text_fun = "contour3d([0 1], 1, [0 1], [0 1], [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong size for input arguments #%d: Vector expected"), "contour3d", 3);
text_fun = "contour3d([0 1], [0 1], 1, [0 1], [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: wrong size for input arguments #%d: a %d-by-%d-by-%d matrix expected"), "contour3d", 3, 2, 2, 2);
text_fun = "contour3d([0 1], [0 1], [0 1], [0 1], [0 1])";
assert_checkerror(text_fun, err_msg);

err_msg = msprintf(_("%s: Wrong size for input arguments #%d: a %d-by-%d-by-%d matrix expected"), "contour3d", 3, 2, 2, 2);
text_fun = "contour3d([0 1], [0 1], [0 1], matrix(1:12, [2 2 3]), [0 1])";
assert_checkerror(text_fun, err_msg);

// Nominal behaviour
implicitplot3d("x^2 + y^2 + z^2 - 1");
ent = gce();

calc = (ent.data.x).^2 + (ent.data.y).^2 + (ent.data.z).^2 - 1;
errQuad = norm(calc) / size(calc, '*');

// Relative error is under 1%
assert_checktrue(errQuad < 0.01);
