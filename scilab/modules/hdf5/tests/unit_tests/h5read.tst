// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5read", 1, 6);
assert_checkerror("h5read()",msgerr,77);
msgerr = msprintf(gettext("%s: Invalid number of input argument.\n"), "h5read");
assert_checkerror("h5read(42)",msgerr,999);
assert_checkerror("h5read(""42"")",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5read", 1);
assert_checkerror("h5read(42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Second argument must be a dataset name.\n"), "h5read");
assert_checkerror("h5read(""42"",42)",msgerr,999);

x = int8(matrix(1:80, 10, 8));
save(TMPDIR + "/x.sod", "x"); // SOD files are HDF5 ones
y = [1 2;3 4];
save(TMPDIR + "/y.sod", "y");

a = h5open(TMPDIR + "/x.sod");
msgerr = msprintf(gettext("%s: %s\n"), "h5read", gettext("Invalid object: not a dataset."));
assert_checkerror("h5read(a)",msgerr,999);
assert_checkequal(h5read(a, "/x"),x);
assert_checkequal(h5read(a, "/x", [3 4], [5 3]),x(4:6,3:7));
msgerr = msprintf(gettext("%s: %s\n"), "h5read", gettext("Invalid selection."));
assert_checkerror("h5read(a,""/x"", [7 9], [10 12])",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5read", msprintf(gettext("Invalid name: %s."), "/y"));
assert_checkerror("h5read(a,""/y"")",msgerr,999);
assert_checkequal(h5read(a, "/x",[2 3],[3 3],[3 2]),int8([13 43 73;15 45 75;17 47 77]));
msgerr = msprintf(gettext("%s: %s\n"), "h5read", gettext("Invalid selection."));
assert_checkerror("h5read(a,""/x"",[2 3],[3 3],[3 5])",msgerr,999);
h5close(a);

msgerr = msprintf(gettext("%s: Can not print H5Object: invalid object.\n"), "h5read");
assert_checkerror("h5read(a)",msgerr,999);
assert_checkequal(h5read(TMPDIR+"/y.sod","/y"),[1,2;3,4]);
