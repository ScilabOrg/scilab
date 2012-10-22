// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
w = "testing";
x = 1:10;
y = [1 2;3 4];
z = 1;
save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");
save(TMPDIR + "/y.sod", "y");
save(TMPDIR + "/z.sod", "z");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkequal(h5isSpace(a),%F);
assert_checkequal(h5isSpace(a.root),%F);
assert_checkequal(h5isSpace(a.root.w),%F);
assert_checkequal(h5isSpace(a.root.w.dataspace)),%T;
h5close(a);

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkequal(h5isSpace(b),%F);
assert_checkequal(h5isSpace(b.root),%F);
assert_checkequal(h5isSpace(b.root.y),%F);
assert_checkequal(h5isSpace(b.root.y.dataspace)),%T;
h5close(b);

c = h5open(TMPDIR + "/y.sod", "r");
assert_checkequal(h5isSpace(c),%F);
assert_checkequal(h5isSpace(c.root),%F);
assert_checkequal(h5isSpace(c.root.y),%F);
assert_checkequal(h5isSpace(c.root.y.dataspace)),%T;
h5close(c);

d = h5open(TMPDIR + "/z.sod", "r");
assert_checkequal(h5isSpace(d),%F);
assert_checkequal(h5isSpace(d.root),%F);
assert_checkequal(h5isSpace(d.root.z),%F);
assert_checkequal(h5isSpace(d.root.z.dataspace)),%T;
h5close(d);


assert_checkequal(deletefile(TMPDIR+"/w.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/x.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/y.sod"),%T);
assert_checkequal(deletefile(TMPDIR+"/z.sod"),%T);
