// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

x = int8(matrix(1:80, 10, 8));
save(TMPDIR + "/x.sod", "x");

msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5ls");
assert_checkerror("h5ls(12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: x"), "h5ls");
assert_checkerror("h5ls(""x"")",msgerr,999);

msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5ls");
assert_checkerror("h5ls(TMPDIR+""/x.sod"",12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5ls");
assert_checkerror("h5ls(TMPDIR+""/x.sod"",""."",12.0)",msgerr,999);

a = h5open(TMPDIR + "/x.sod");
dset = ["x" "dataset"];
attr = ["SCILAB_scilab_version" "attribute";"SCILAB_sod_version" "attribute"];
assert_checkequal(h5ls(a),cat(1,dset,attr));
attr = ["SCILAB_Class" "attribute";"SCILAB_precision" "attribute"];
assert_checkequal(h5ls(a.root.x),attr);
assert_checkequal(h5ls(a,"/x"),attr);

attr = ["SCILAB_scilab_version";"SCILAB_sod_version"];
assert_checkequal(h5ls(a,".","a"),attr);

msgerr = msprintf(gettext("%s: Invalid filter"), "h5ls");
assert_checkerror("h5ls(a,""."",""x"")",msgerr,999);

msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5ls");
assert_checkerror("h5ls(12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5ls");
assert_checkerror("h5ls(a,12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5ls");
assert_checkerror("h5ls(a,""."",12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: x"), "h5ls");
assert_checkerror("h5ls(""x"")",msgerr,999);

h5close(a);

attr = ["SCILAB_scilab_version" "attribute";"SCILAB_sod_version" "attribute"];
assert_checkequal(h5ls(TMPDIR+"/x.sod"),cat(1,dset,attr));
attr = ["SCILAB_Class" "attribute";"SCILAB_precision" "attribute"];
assert_checkequal(h5ls(TMPDIR+"/x.sod","/x"),attr);
attr = ["SCILAB_scilab_version";"SCILAB_sod_version"];
assert_checkequal(h5ls(TMPDIR+"/x.sod",".","a"),attr);

msgerr = msprintf(gettext("%s: Invalid filter"), "h5ls");
assert_checkerror("h5ls(TMPDIR+""/x.sod"",""."",""x"")",msgerr,999);


