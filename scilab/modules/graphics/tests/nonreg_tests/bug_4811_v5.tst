// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 4811 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4811
//
// <-- Short Description -->
//   for some figures, it is not possible to save and load the handle of the figure.

warning("off");

plot3d();
h = gcf();

tmp_file=pathconvert(TMPDIR+'/nonreg_test_bug4811.dat',%f);

save(tmp_file,h);
// close the figure
delete(h);
load(tmp_file);

