// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2012 - Digiteo - Vincent Couvert
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demo_plot3d = scf(100002);

// Test set functions for uicontrol
h = uicontrol("style", "imagerenderer");
set(h, "string", SCI + "/modules/demo_tools/images/logo_scilab.png");
set(h, "position", [0 0 300 300]);
set(h, "value", [0.3 2 0 0 0]);
set(h, "value", [0.3 2 0.1 2 0]);
set(h, "value", [0.3 2 0.1 2 30]);