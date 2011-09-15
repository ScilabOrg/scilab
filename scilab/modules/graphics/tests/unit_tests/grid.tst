// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test grid property

f = gcf();
a = gca();
assert_checkequal(a.view, "2d");
assert_checkequal(a.grid, [-1, -1]);
a.grid = [1 2 3];
assert_checkequal(a.grid, [1, 2]);
a.view = "3d";
assert_checkequal(a.grid, [1, 2, 3]);
a.grid = [1 1 -1];
assert_checkequal(a.grid, [1, 1, -1]);
a.view = "2d";
assert_checkequal(a.grid, [1, 1]);
a.view = "3d";
assert_checkequal(a.grid, [1, 1, -1]);
