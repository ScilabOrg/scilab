// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Test text transparency.

a = gca();
a.font_size = 6;

xstring(0,0,"toto");
e = gce();
e.data = [0.5,0.5,0.8];

xstring(0,0,"toto");
e = gce();
e.data = [0.6,0.5,0.0];


xstring(0,0,"toto");
e = gce();
e.data = [0.4,0.4,-0.6];

a = gca();
a.view = "3d";

// check that text is transparent
for i = 1:360,
  a.rotation_angles(2) = i;
end



