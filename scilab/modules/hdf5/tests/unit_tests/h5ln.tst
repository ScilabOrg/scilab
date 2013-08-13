// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5ln", 3, 5);
assert_checkerror("h5ln()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ln", 1);
assert_checkerror("h5ln(42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ln", 2);
assert_checkerror("h5ln(""42"",42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ln", 3);
assert_checkerror("h5ln(""42"",""42"",42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5ln", msprintf(gettext("Invalid hdf5 file: %s."), "42"));
assert_checkerror("h5ln(""42"",""42"",""42"")",msgerr,999);

a = h5open(TMPDIR + "/test.h5");
h5ln(a,"42","42");
msgerr = msprintf(gettext("%s: Error in retrieving field content:\n%s\n"), "%H5Object_e", msprintf(gettext("Invalid name: %s."), "/42"));
msgerr($+1) = gettext("HDF5 description") + ": " + "too many links.";
assert_checkerror("a(""/42"")",msgerr,999);
h5rm(a,"/42");

b = h5open(TMPDIR + "/test1.h5");

h5group(a, "Grp_1");
h5write(a.root.Grp_1, "Dset_1", [1 2 ; 3 4]);
h5group(a, "Grp_3");
h5write(a.root.Grp_3, "Dset_3", [16 32; 42 84]);
h5group(b, "BGrp");
h5write(b("/BGrp"), "BDset", 11:18);
h5group(b, "BGrp_2");
h5write(b("/BGrp_2"), "BDset_2", 8:12);

h5close(b)

h5group(a, "Grp_2");
h5ln(a, "Hard_Link", "/Grp_1/Dset_1", %t)
assert_checkequal(a.root.Datasets,"Hard_Link");
h5ln(a, "Soft_Link", "/Grp_1/Dset_1")
assert_checkequal(a.root.Softs,"Soft_Link");
assert_checkequal(a.root.Datasets,["Hard_Link";"Soft_Link"]);
h5ln(a, "External_Link", TMPDIR  +"/test1.h5", "/BGrp");
assert_checkequal(a.root.Externals,"External_Link");
h5ln(a, "External_Link2", TMPDIR  +"/test1.h5", "/BGrp/BDset");
assert_checkequal(a.root.Datasets,["Hard_Link";"Soft_Link";"External_Link2"]);

h5rm(a,'/Grp_1/Dset_1');
assert_checkequal(a.root.Datasets,["Hard_Link";"External_Link2"]);
assert_checkequal(a.root.Softs,["Soft_Link"]);
msgerr = msprintf(gettext("%s: Error in retrieving field content:\n%s\n"), "%H5Object_e", msprintf(gettext("Invalid field: %s"), "Soft_Link"));
assert_checkerror("a.root.Soft_Link",msgerr,999);
h5rm(a,'/Hard_Link');
assert_checkequal(a.root.Datasets,["External_Link2"]);

h5close(a);
