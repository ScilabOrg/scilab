// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = h5open(TMPDIR + "/test.h5", "w")
h5group(a, "/mnt");
b = h5open(TMPDIR + "/test1.h5", "w")
h5group(b, "Group_1");
h5write(b, "Group_1/Dataset_1", [1 2;3 4]);
c = h5open(TMPDIR + "/test2.h5", "w")
h5group(c, "Group_2");
h5write(c, "Group_2/Dataset_2", [14 15 16;17 18 19]);
d = h5open(TMPDIR + "/test3.h5", "w")
h5group(d, "Group_3");
h5write(d, "Group_3/Dataset_3", [10 11;17 18 19]);

h5mount(a, "/mnt", b);
assert_checkequal(a.root.mnt.Groups,"Group_1");
msgerr = msprintf(gettext("%s: Cannot mount the file: %s"), "h5mount",TMPDIR+"/test1.h5");
assert_checkerror("h5mount(a, ""/mnt"", b)",msgerr,999);
h5mount(a, "/mnt", c);
assert_checkequal(a.root.mnt.Groups,"Group_2");


a.root.mnt.Group_1.Dataset_1.data
h5write(a.root.mnt.Group_1, "Dataset_1", [10 11;12 13])
h5umount(a, "/mnt")
h5rm(a, "/mnt")
h5close(a, b)

// Check that the modification in b is ok
h5dump(TMPDIR + "/test1.h5", "/Group_1/Dataset_1")
