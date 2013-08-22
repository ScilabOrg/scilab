// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2112 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2112
//
// <-- Short Description -->
// GIF export may alter some update

driver("GIF");
x=[-3:0.1:3];
y=[-3:0.1:3];
cd TMPDIR;
xinit("test.gif");
plot(x,y)
zoom_rect([-2 -2 2 2])
f=gcf();
x_ticks_location = f.children(1).x_ticks.locations(1);
xend();

if (x_ticks_location ~= -2) then pause; end
mdelete("test.gif");
