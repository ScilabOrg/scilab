// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3999 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3999
//
// <-- Short Description -->
// xbasc() doesn't clear the current graphics window.
// xbasc replaced by clf
//

x = 1:10;
plot(x);
clf();

// check that there only an axes below the current figure
fig = gcf();
// only axes
assert_checkequal(size(fig.children), [1,1]);
// no children below axes
assert_checkequal(fig.children(1).children, []);

// same with other version of xbasc
plot(x);
plot3d(x,x,cos(x)'*sin(x));

// erase all figure with index from 0 to 3 even if some of them do no exist
clf(0:3);

// Current figure must be empty
fig = get_figure_handle(0);
assert_checkequal(size(fig.children), [1,1]); // Default axes
// no children below axes
assert_checkequal(fig.children(1).children, []);
