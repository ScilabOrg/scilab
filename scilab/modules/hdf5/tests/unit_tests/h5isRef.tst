// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
w = "testing";
x = list(1, "hello");
y = [1 2;3 4];
z = 1;
save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");
save(TMPDIR + "/y.sod", "y");
save(TMPDIR + "/z.sod", "z");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkequal(h5isRef(a),%F);
aw = a.root.w.data;
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A H5Object expected.\n"), "h5isRef");
assert_checkerror("h5isRef(aw)",msgerr,999);
h5close(a)

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkequal(h5isRef(b),%F);
bx = b.root.x.data;
assert_checkequal(h5isRef(bx),%T);
h5close(b)

c = h5open(TMPDIR + "/y.sod", "r");
assert_checkequal(h5isRef(c),%F);
cy = c.root.y.data;
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A H5Object expected.\n"), "h5isRef");
assert_checkerror("h5isRef(cy)",msgerr,999);
h5close(c)

d = h5open(TMPDIR + "/z.sod", "r");
assert_checkequal(h5isRef(d),%F);
dz = d.root.z.data;
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A H5Object expected.\n"), "h5isRef");
assert_checkerror("h5isRef(dz)",msgerr,999);
h5close(d)

assert_checkequal(deletefile(TMPDIR+"/w.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/x.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/y.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/z.sod"),%T);
