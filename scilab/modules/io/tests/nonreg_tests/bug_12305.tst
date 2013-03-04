// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 12305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12305
//
// <-- Short Description -->
//   Legends could not be loaded from SOD file.

//create a plot with legend
t=linspace(0,%pi,20);
a=gca();
a.data_bounds=[t(1) -1.8;t($) 1.8];
plot2d(t,[cos(t'),cos(2*t'),cos(3*t')],[-5,2 3]);  
e=gce();
e1=e.children(1);
e1.thickness=2;
e1.polyline_style=4;
e1.arrow_size_factor = 1/2;
e.children(2).line_style=4;
e3=e.children(3);
e3.line_mode='on';
e3.mark_background=5;
hl=legend(['cos(t)';'cos(2*t)';'cos(3*t)']);

f=gcf();
//save legend text
ref = f.children(1).children(1).text;
//save current graphics
save(TMPDIR + "/legend.sod", "f");
//clear graphics
delete(gcf());
//clear variables
clear a e e1 e3 f hl;
//load graphics
load(TMPDIR + "/legend.sod");
//check restored values with reference
assert_checkequal(ref,  f.children(1).children(1).text);