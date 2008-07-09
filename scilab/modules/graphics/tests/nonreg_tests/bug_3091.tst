// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3091 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3091
//
// <-- Short Description -->
// There is a white background behind each charater of tics.

// check that figure_size does not change
// between the two calls
fig = scf();
figSize1 = fig.figure_size;

for i = 1:100,
  fig = gcf();
  figSize2 = fig.figure_size;
  if (figSize2 <> figSize1) then pause;end
end

delete(fig);

// same for axes_size
fig = scf();
axesSize1 = fig.axes_size;

for i = 1:100,
  axesSize2 = fig.axes_size;
  if (axesSize2 <> axesSize1) then pause;end
end


delete(fig);

// same for figure_position
fig = scf();
figPos1 = fig.figure_position;

for i = 1:100,
  figPos2 = fig.figure_position;
  if (figPos2 <> figPos1) then pause;end
end



