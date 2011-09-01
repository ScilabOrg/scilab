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

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cmscope.xcos"));
xcos_simulate(scs_m, 4);

f=gcf();
a1=f.children(1);
a2=f.children(2);
a3=f.children(3);

p11=a1.children(1);
p12=a1.children(2);
p21=a2.children(1);
p31=a3.children(1);

assert_checkequal(f.figure_id, 20003);

assert_checkequal(a1.data_bounds, [0 -30 ; 90 30]);
assert_checkequal(a2.data_bounds, [0 -5 ; 60 5]);
assert_checkequal(a3.data_bounds, [0 -1 ; 30 1]);

assert_checkequal(a1.x_label.text, "t");
assert_checkequal(a2.x_label.text, "t");
assert_checkequal(a3.x_label.text, "t");
assert_checkequal(a1.y_label.text, "y");
assert_checkequal(a2.y_label.text, "y");
assert_checkequal(a3.y_label.text, "y");

// check removed due to junk data at the end
// assert_checkequal(size(p11.data), [299 2]);
// assert_checkequal(size(p12.data), [299 2]);
// assert_checkequal(size(p21.data), [299 2]);
// assert_checkequal(size(p31.data), [299 2]);

