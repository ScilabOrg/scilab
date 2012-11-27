// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 1 to 5 expected."), "h5open");
assert_checkerror("h5open()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5open");
assert_checkerror("h5open(42)",msgerr,999);

a = h5open(TMPDIR + "/x.sod");
assert_checkequal(a.root.Name,'/')
h5close(a);
x = 1:10;
save(TMPDIR + "/x.sod", "x");
b = h5open(TMPDIR + "/x.sod");
assert_checkequal(b.root.Datasets,'x');
h5close(b);
a = h5open(TMPDIR + "/y.tst");
assert_checkequal(a.root.Name,'/');
h5write(a,"Dset_1",[1 2;3 4]);
h5close(a);

msgerr = msprintf(gettext("%s: Cannot append the file (not HDF5): %s."), "h5open", "/");
assert_checkerror("h5open(""/"")",msgerr,999);
msgerr = msprintf(gettext("%s: Cannot append the file (not HDF5): %s."), "h5open", SCI + "/COPYING-FR");
assert_checkerror("h5open(SCI + ""/COPYING-FR"")",msgerr,999);
copyfile(SCI+"/COPYING-FR",TMPDIR+"/z.h5");
msgerr = msprintf(gettext("%s: Cannot append the file (not HDF5): %s."), "h5open", TMPDIR + "/z.h5");
assert_checkerror("h5open(TMPDIR + ""/z.h5"")",msgerr,999);
