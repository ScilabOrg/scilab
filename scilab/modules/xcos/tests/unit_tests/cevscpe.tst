// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
// <-- TEST WITH GRAPHIC -->

// test scope values 

loadXcosLibs();

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cevscpe.xcos"));
xcos_simulate(scs_m, 4);

f=gcf();
a=gca();
s1=a.children(1);
s2=a.children(2);
s3=a.children(3);

assert_checkequal(f.figure_id, 20007);
assert_checkequal(a.data_bounds, [30 0 ; 60 1]);
assert_checkequal(a.x_label.text, "t");
assert_checkequal(a.y_label.text, "y");

assert_checkequal(size(s1.data), [26 2]);
assert_checkequal(size(s2.data), [26 2]);
assert_checkequal(size(s3.data), [26 2]);

assert_checkequal(s1.visible, "on");
assert_checkequal(s2.visible, "on");
assert_checkequal(s3.visible, "on");

assert_checkequal(s1.line_mode, "on");
assert_checkequal(s2.line_mode, "on");
assert_checkequal(s3.line_mode, "on");

assert_checkequal(s1.line_style, 1);
assert_checkequal(s2.line_style, 1);
assert_checkequal(s3.line_style, 1);

assert_checkequal(s1.thickness, 1);
assert_checkequal(s2.thickness, 1);
assert_checkequal(s3.thickness, 1);

assert_checkequal(s1.arrow_size, 0);
assert_checkequal(s2.arrow_size, 0);
assert_checkequal(s3.arrow_size, 0);

assert_checkequal(s1.segs_color, 5*ones(1,13));
assert_checkequal(s2.segs_color, 3*ones(1,13));
assert_checkequal(s3.segs_color, 1*ones(1,13));


