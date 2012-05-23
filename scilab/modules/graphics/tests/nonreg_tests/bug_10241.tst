// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//
// <-- Non-regression test for bug 10241 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10241
//
// <-- Short Description -->
// Can not open a .scg file when a non-ascii character is in the legend.

// Old binary format save/load
f=scf(0);
x=[-5:.1:5]';
plot2d(x,[2*x 3*x]);
legend(["2x (�m)","3x (�m)"],2);
warning("off");
save("TMPDIR/bug_10241.scg", f);
warning("on");
delete(f);
load("TMPDIR/bug_10241.scg");
a=gca();
assert_checkequal(a.children(1).text, ["2x (�m)","3x (�m)"]');
delete(gcf());

// SOD format save/load
f=scf(0);
x=[-5:.1:5]';
plot2d(x,[2*x 3*x]);
legend(["2x (�m)","3x (�m)"],2);
save("TMPDIR/bug_10241.scg", "f");
delete(f);
load("TMPDIR/bug_10241.scg");
a=gca();
assert_checkequal(a.children(1).text, ["2x (�m)","3x (�m)"]');
delete(gcf());
