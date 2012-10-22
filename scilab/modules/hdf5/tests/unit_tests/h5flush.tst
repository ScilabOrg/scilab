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
a.root.Dset1.Data
//assert_checkequal(disp(a.root.Datasets),"Dset1");
h5close(a)
a=h5open(TMPDIR+"/test.h5")
a.root.Dset1.Data
