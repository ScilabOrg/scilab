// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

warning("off");

// test to check that save and load are working properly
plotExportFile = pathconvert(TMPDIR) + "savePlot.scg";

// check clip properties
clf();
x = 0:10;
z = sin(x)'*cos(x);
plot3d(x, x, z);
e = gce();
e.clip_state = "clipgrf";
plot3d(x ,x ,z + 1);
e = gce();
e.clip_box = [1, 2, 3, 4];
e.clip_state = "on";

// save the two curves
save(plotExportFile, gcf());

// close window
delete(gcf());

// reload data
load(plotExportFile);
axes = gca();

// check axes properties
surf1 = axes.children(1);
assert_checkequal(surf1.clip_box, [1, 2, 3, 4]);
assert_checkequal(surf1.clip_state, "on");
surf2 = axes.children(2);
assert_checkequal(surf2.clip_box, []);
assert_checkequal(surf2.clip_state, "clipgrf");

// same for grayplot
clf();
x = 0:10;
z = sin(x)'*cos(x);
grayplot(x, x, z)
e = gce();
e.clip_state = "clipgrf";
grayplot(x, x, z + 1);
e = gce();
e.clip_box = [1, 2, 3, 4];
e.clip_state = "on";

// save the two curves
save(plotExportFile, gcf());

// close window
delete(gcf());

// reload data
load(plotExportFile);
axes = gca();

// check axes properties
surf1 = axes.children(1);
if (surf1.clip_box <> [1, 2, 3, 4]) then pause; end
if (surf1.clip_state <> "on") then pause; end
surf2 = axes.children(2);
if (surf2.clip_box <> []) then pause; end
if (surf2.clip_state <> "clipgrf") then pause; end

// same for Matplot
clf();
x = 0:10;
z = sin(x)'*cos(x);
Matplot(z)
e = gce();
e.clip_state = "clipgrf";
Matplot(z + 1);
e = gce();
e.clip_box = [1, 2, 3, 4];
e.clip_state = "on";

// save the two curves
save(plotExportFile, gcf());

// close window
delete(gcf());

// reload data
load(plotExportFile);
axes = gca();

// check axes properties
surf1 = axes.children(1);
if (surf1.clip_box <> [1, 2, 3, 4]) then pause; end
if (surf1.clip_state <> "on") then pause; end
surf2 = axes.children(2);
if (surf2.clip_box <> []) then pause; end
if (surf2.clip_state <> "clipgrf") then pause; end


// same for fec
clf();
x = 0:10;
z = sin(x)'*cos(x);
Sgrayplot(x, x, z)
e = gce();
e = e.children(1);
e.clip_state = "clipgrf";
Sgrayplot(x, x, z + 1)
e = gce();
e = e.children(1);
e.clip_box = [1, 2, 3, 4];
e.clip_state = "on";

// save the two curves
save(plotExportFile, gcf());

// close window
delete(gcf());

// reload data
load(plotExportFile);
axes = gca();

// check axes properties
surf1 = axes.children(1).children(1);
if (surf1.clip_box <> [1, 2, 3, 4]) then pause; end
if (surf1.clip_state <> "on") then pause; end
surf2 = axes.children(2).children(1);
if (surf2.clip_box <> []) then pause; end
if (surf2.clip_state <> "clipgrf") then pause; end

// datatip test
x=linspace(0,1,9)';
y=x.^3;
clf();
plot(x,y);
e=gce();p=e.children(1);//get the handle on the polyline
p.mark_mode="on";p.mark_style=2;p.mark_size=12;
t=datatipCreate(p,5);

// save the curves
f = gcf();
save(plotExportFile, "f");
// close window
delete(f);
// reload data
load(plotExportFile);

e = gce();
e = e.children(1);
assert_checkequal(e.datatips.data, [0.5, 0.125, 0]);
