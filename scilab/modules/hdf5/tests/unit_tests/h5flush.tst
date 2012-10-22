// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = h5open(TMPDIR + "/test.h5", "w")
x = uint8(1:24);
h5dataset(a, "Dset1", [1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1; 1 2 3 4], x, [1 2 3 4 ; 1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1 ; 1 2 3 4]);
assert_checkequal(h5ls(a.root.Datasets),["Dset1" "dataset"]);

b = h5open(TMPDIR + "/test1.h5");
h5group(a, "Grp_1");
h5write(a.root.Grp_1, "Dset_1", [1 2 ; 3 4]);
//h5flush(a);
h5group(b, "BGrp");
h5write(b("/BGrp"), "BDset", 11:18);
h5close(b)

h5mv(a.root.Grp_1, "Dset_1", a, "Dset_2")
assert_checkequal(a.root.Datasets,"Dset_2");
assert_checkequal(a.root.Dset_2,[1 2;3 4]);
