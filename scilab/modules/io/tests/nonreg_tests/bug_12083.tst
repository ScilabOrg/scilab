// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12083 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12083
//
// <-- Short Description -->
// Load multiple variables could failed due to missing "clear" of previous variables.

plot3d();
f=gcf();

//save f, gcf, %_plot3d, ...
save(TMPDIR + "/bug_12083.sod");
disp(TMPDIR);

//close Scilab and retart it
LOAD = "/path/to/saved/file":
load(LOAD + "/bug_12083.sod");
f=gcf();
assert_checkequal(f.children(1).children(1).type, "Plot3d");