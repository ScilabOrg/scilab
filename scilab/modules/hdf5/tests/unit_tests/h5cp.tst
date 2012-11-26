// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = h5open(TMPDIR + "/test.h5");
b = h5open(TMPDIR + "/test1.h5");
c = h5open(TMPDIR + "/test2.h5");
h5group(a, "Grp_1");
h5write(a.root.Grp_1, "Dset_1", [1 2 ; 3 4]);
h5flush(a);
h5group(b, "BGrp");
h5write(b("/BGrp"), "BDset", 11:18);
h5close(b);
h5group(c, "Grp_3");
h5group(c, "Grp_4");
h5write(c.root.Grp_3,"Dset_3",[5 6 ; 7 8]);

h5cp(a.root.Grp_1, "Dset_1", a, "Dset_2");
assert_checkequal(a.root.Datasets,["Dset_2"]);

h5cp(a.root.Grp_1, "Dset_1", a);
assert_checkequal(a.root.Datasets,["Dset_1" ; "Dset_2"]);

h5group(a,"/Grp_1/SubGrp_1");
h5cp(a, "/Grp_1/SubGrp_1", a);
assert_checkequal(a.root.Groups,["Grp_1" ; "SubGrp_1"]);

h5cp(a.root.Grp_1, c, "Dset_1");
assert_checkequal(c.root.Groups,["Dset_1" ; "Grp_3" ; "Grp_4"]);

h5cp(c.root.Grp_3,c.root.Grp_4);
assert_checkequal(c.root.Grp_4.Groups,["Grp_3"]);

h5flush(c);
h5close(c);

h5cp(TMPDIR  +"/test1.h5", "/BGrp/BDset", a.root.Grp_1);
assert_checkequal(a.root.Grp_1.Datasets,["BDset" ; "Dset_1"]);

h5cp(TMPDIR  +"/test1.h5", "/BGrp/BDset", a.root.Grp_1, "The Foo Dataset");
assert_checkequal(a.root.Grp_1.Datasets,["BDset" ; "Dset_1" ; "The Foo Dataset"]);

h5cp(TMPDIR +"/test2.h5", "/Grp_4",TMPDIR +"/test1.h5","/CGrp");
assert_checkequal(h5ls(TMPDIR+"/test1.h5",".","g"),["BGrp" ; "CGrp"]);

msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5cp");
assert_checkerror("h5cp(12.0,14.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5cp");
assert_checkerror("h5cp(""12.0"",14.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s"), "h5cp", "12.0");
assert_checkerror("h5cp(""12.0"",""14.0"")",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: empty filename"), "h5cp");
assert_checkerror("h5cp(a,""14.0"")",msgerr,999);

h5close(a);
