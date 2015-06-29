// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015-2010 - Juergen Koch <juergen.koch@hs-esslingen.de>
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

//
// Test data
t = 1:200;
x = t.*cos(t).*sin(t);
y = t.*sin(t).*sin(t);
z = t.*cos(t);
s = linspace(50,1,length(t));

//
// Test #1 Create 3D scatter plot 
h = scf();
p = scatter3(x,y,z);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
set(gca(),"rotation_angles",[60,45]);
close(h);
//
// Test #2 Vary marker size 
h = scf();
p = scatter3(x,y,z,s);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
set(gca(),"rotation_angles",[60,45]);
close(h);
//
// Test #3 Vary marker size and color
h = scf();
set(gcf(),"color_map",coolcolormap(64));
p = scatter3(x,y,z,s,t);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,floor((64-1)*(t-min(t))/(max(t)-min(t))+1));
assert_checkequal(p.mark_background,0);
set(gca(),"rotation_angles",[60,45]);
close(h);

//
// Test data
z = linspace(0,25,200);
x = z.*cos(z);
y = z.*sin(z);

//
// Test #4 Fill the markers
h = scf();
p = scatter3(x,y,z,"fill");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,2);
set(gca(),"rotation_angles",[60,45]);
close(h);
//
// Test #5 Specify marker symbol
h = scf();
p = scatter3(x,y,z,"*");
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,10);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,2);
assert_checkequal(p.mark_background,0);
set(gca(),"rotation_angles",[60,45]);
close(h);
//
// Test #6 Change marker edge and face color
h = scf();
p = scatter3(x,y,z,...
         "markerEdgeColor","black",...
         "markerFaceColor",[0 .8 .8]);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,7);
assert_checkequal(p.mark_foreground,color("black"));
assert_checkequal(p.mark_background,color(0,.8*255,.8*255));
set(gca(),"rotation_angles",[60,45]);
close(h);
//
// Test #7 Specify subplot for scatter plot
h = scf();
[x,y] = meshgrid(linspace(-2,2,20));
z = exp(-x.^2-y.^2);
subplot(2,1,1);
p1 = scatter3(gca(),x(:),y(:),z(:));
assert_checkequal(p1.line_mode,"off");
assert_checkequal(p1.fill_mode,"off");
assert_checkequal(p1.thickness,1);
assert_checkequal(p1.mark_mode,"on");
assert_checkequal(p1.mark_style,9);
assert_checkequal(p1.mark_size_unit,"point");
assert_checkequal(p1.mark_size,7);
assert_checkequal(p1.mark_foreground,2);
assert_checkequal(p1.mark_background,0);
set(gca(),"rotation_angles",[60,45]);
subplot(2,1,2);
p2 = scatter3(gca(),x(:),y(:),z(:),"markerFaceColor",[0 .8 .8]);
assert_checkequal(p2.line_mode,"off");
assert_checkequal(p2.fill_mode,"off");
assert_checkequal(p2.thickness,1);
assert_checkequal(p2.mark_mode,"on");
assert_checkequal(p2.mark_style,9);
assert_checkequal(p2.mark_size_unit,"point");
assert_checkequal(p2.mark_size,7);
assert_checkequal(p2.mark_foreground,2);
assert_checkequal(p2.mark_background,color(0,.8*255,.8*255));
set(gca(),"rotation_angles",[60,45]);
close(h);

//
// Test data
t = 1:200;
x = t.*cos(t).*sin(t);
y = t.*sin(t).*sin(t);
z = t.*cos(t);
s = linspace(50,1,length(t));

//
// Test #8 Modify scatter plot after creation
h = scf();
p = scatter3(x,y,z,s);
p.mark_foreground = addcolor([0.5 0 0]);
p.mark_background = addcolor([0.5 0.5 0]);
assert_checkequal(p.line_mode,"off");
assert_checkequal(p.fill_mode,"off");
assert_checkequal(p.thickness,1);
assert_checkequal(p.mark_mode,"on");
assert_checkequal(p.mark_style,9);
assert_checkequal(p.mark_size_unit,"point");
assert_checkequal(p.mark_size,ceil(sqrt(4*s/%pi)));
assert_checkequal(p.mark_foreground,color(0.5*255,0,0));
assert_checkequal(p.mark_background,color(0.5*255,0.5*255,0));
set(gca(),"rotation_angles",[60,45]);
close(h);
