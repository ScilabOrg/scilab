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

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cscope.xcos"));
xcos_simulate(scs_m, 4);

f=gcf();
a=gca();
p1=a.children(1);
p2=a.children(2);
p3=a.children(3);
p4=a.children(4);

assert_checkequal(f.figure_id, 20006);
assert_checkequal(a.data_bounds, [30 -2 ; 60 3]);
assert_checkequal(a.x_label.text, "t");
assert_checkequal(a.y_label.text, "y");

// check removed due to junk data at the end
// assert_checkequal(size(p1.data), [315 2]);
// assert_checkequal(size(p2.data), [315 2]);
// assert_checkequal(size(p3.data), [315 2]);
// assert_checkequal(size(p4.data), [315 2]);

assert_checkequal(p1.polyline_style, 1);
assert_checkequal(p2.polyline_style, 1);
assert_checkequal(p3.polyline_style, 1);
assert_checkequal(p4.polyline_style, 1);

assert_checkequal(p1.line_mode, "on");
assert_checkequal(p2.line_mode, "on");
assert_checkequal(p3.line_mode, "off");
assert_checkequal(p4.line_mode, "on");

assert_checkequal(p1.mark_mode, "off");
assert_checkequal(p2.mark_mode, "off");
assert_checkequal(p3.mark_mode, "on");
assert_checkequal(p4.mark_mode, "off");

