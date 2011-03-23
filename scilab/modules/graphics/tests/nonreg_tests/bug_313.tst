// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 313 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=313
//
// <-- Short Description -->
// clipping does not work in 2.6

// define clip box
clipBox = [150, 460, 100, 150];

x=0:0.2:2*%pi;
x1=[sin(x);100*sin(x)];
y1=[cos(x);100*cos(x)];
y1=y1+20*ones(y1);
// No clip 
plot2d([-100,500],[-100,600],[-1,-1],strf="022");
xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1));
// rectangle clipping zone 
clf(); plot2d([-100,500],[-100,600],[-1,-1],strf="022")
xrect(clipBox(1), clipBox(2), clipBox(3), clipBox(4));
axes = gca();
axes.clip_box = clipBox;
axes.clip_state = "on";
xsegs(10*x1+200*ones(x1),10*y1+200*ones(y1));

// check that xsegs is clipped
segs = gce();
if (segs.clip_state <> "on") then pause; end;
if (segs.clip_box <> clipBox) then pause; end

