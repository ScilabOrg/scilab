// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 5148 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5148
//
// <-- Short Description -->
//   Setting {x,y,z}_ticks with empty value hangs.


a=gca();
a.x_ticks=tlist(["ticks"  "locations"  "labels"], [], []);
a.y_ticks=tlist(["ticks"  "locations"  "labels"], [], []);
a.z_ticks=tlist(["ticks"  "locations"  "labels"], [], []);

a.x_ticks=tlist(["ticks"  "locations"  "labels"], [0,1], ["e","f"]);
a.y_ticks=tlist(["ticks"  "locations"  "labels"], [0,1], ["e","f"]);
a.z_ticks=tlist(["ticks"  "locations"  "labels"], [0,1], ["e","f"]);
