// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

x = 1:5 + %pi;
save(TMPDIR + "/x.sod", "x");
a = h5open(TMPDIR + "/x.sod");
h5attr(a, ".", "My Attribute", int8(123));
assert_checkequal(a.root("My Attribute").Data,int8(123));
msgerr = msprintf(gettext("%s: Cannot create a new attribute."), "h5attr");
assert_checkerror("h5attr(a, ""."", ""My Attribute"", %pi)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5attr");
assert_checkerror("h5attr(a,""."",a.root.Attributes,int8(2))",msgerr,999);

h5group(a,"Group_1");
h5attr(a,"/Group_1","New",[5 6]);
assert_checkequal(a.root.Group_1.New.Data,[5 6]')
h5write(a,"Group_1/Dset_1",[1 2;3 4]);
h5attr(a,"Group_1/Dset_1","Second",[7 8]);
assert_checkequal(a.root.Group_1.Dset_1.Second.Data,[7 8]');

msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s"), "h5attr",TMPDIR+"/y.sod");
assert_checkerror("h5attr(TMPDIR+""/y.sod"",""."",""New"",int8(16))",msgerr,999);
h5attr(TMPDIR+"/x.sod", ".", "New", int8(8));
assert_checkequal(a.root.New.Data,int8(8));


h5dump(a)
a.root("My Attribute").data
h5attr(a, "/x", "My Dataset Attribute", int16(321));
h5dump(a.root.x)
h5close(a);
