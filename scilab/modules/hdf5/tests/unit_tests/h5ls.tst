// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5ls", 1, 3);
assert_checkerror("h5ls()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 1);
assert_checkerror("h5ls(42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5ls", msprintf(gettext("Invalid hdf5 file: %s."), "42"));
assert_checkerror("h5ls(""42"")",msgerr,999);

x = int8(matrix(1:80, 10, 8));
save(TMPDIR + "/x.sod", "x");


msgerr = msprintf(gettext("%s: %s\n"), "h5ls", msprintf(gettext("Invalid hdf5 file: %s."), "x"));
assert_checkerror("h5ls(""x"")",msgerr,999);

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 2);
assert_checkerror("h5ls(TMPDIR+""/x.sod"",12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 3);
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

msgerr = msprintf(gettext("%s: %s\n"), "h5ls", gettext("Invalid filter"));
assert_checkerror("h5ls(a,""."",""x"")",msgerr,999);

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 1);
assert_checkerror("h5ls(12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 2);
assert_checkerror("h5ls(a,12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5ls", 3);
assert_checkerror("h5ls(a,""."",12.0)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5ls", msprintf(gettext("Invalid hdf5 file: %s."), "x"));
assert_checkerror("h5ls(""x"")",msgerr,999);

h5close(a);

attr = ["SCILAB_scilab_version" "attribute";"SCILAB_sod_version" "attribute"];
assert_checkequal(h5ls(TMPDIR+"/x.sod"),cat(1,dset,attr));
attr = ["SCILAB_Class" "attribute";"SCILAB_precision" "attribute"];
assert_checkequal(h5ls(TMPDIR+"/x.sod","/x"),attr);
attr = ["SCILAB_scilab_version";"SCILAB_sod_version"];
assert_checkequal(h5ls(TMPDIR+"/x.sod",".","a"),attr);



