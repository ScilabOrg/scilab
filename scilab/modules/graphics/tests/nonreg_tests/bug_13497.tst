// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13497 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13497
//
// <-- Short Description -->
// newaxes did not create axes in the current window if its default_axes property was off.

// No open window, newaxes() creates one.
newaxes();
assert_checkequal(size(winsid(), "*"), 1);
close;

// One open window but with default_axes "off", create axes in it.
f = figure("default_axes", "off");
newaxes();
assert_checkequal(size(winsid(), "*"), 1);
close;

// One open window with default_axes "on", create new axes in it.
f = figure();
newaxes();
assert_checkequal(size(winsid(), "*"), 1);
close;
