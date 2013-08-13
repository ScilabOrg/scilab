// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5dataset", 5, 7);
assert_checkerror("h5dataset()",msgerr,77);
assert_checkerror("h5dataset(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string or a H5Object expected.\n"), "h5dataset", 1);
assert_checkerror("h5dataset(42,42,42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "h5dataset", 2);
assert_checkerror("h5dataset(""42"",42,42,42,42)",msgerr,999);
//msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: five row vector expected.\n"), "h5dataset", 3);
//assert_checkerror("h5dataset(""42"",""42"",42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A double matrix expected.\n"), "h5dataset", 3);
assert_checkerror("h5dataset(""42"",""42"",[""42"",""42"",""42"",""42"",""42""],42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d: six row vector expected.\n"), "h5dataset", 5);
assert_checkerror("h5dataset(""42"",""42"",[42;42;42;42;42],42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5dataset", msprintf(gettext("Invalid hdf5 file: %s."), "42"));
assert_checkerror("h5dataset(""42"",""42"",[42;42;42;42;42],42,[42;42;42;42;42;42])",msgerr,999);

a = h5open(TMPDIR + "/test.h5", "w");

msgerr = msprintf(gettext("%s: %s\n"), "h5dataset", gettext("Incompatible dimensions"));
assert_checkerror("h5dataset(a,""42"",[42;42;42;42;42],42,[42;42;42;42;42;42])",msgerr,999);

x = uint32(1:100);
y = uint32(matrix(1:100,10,10));

z = uint32(zeros(7,12));
h5write(a,"dx",y',[2 1 ],[ 4 1 ],[ 2 1 ],[ 1 9 ]);
z(3:6,3:11) = a.root.dx.Data;

msgerr = msprintf(gettext("%s: %s\n"), "h5dataset", msprintf(gettext("Cannot create the dataset: %s"), ""));
assert_checkerror("h5dataset(a, """", [10 10 ; 2 1 ; 4 1 ; 2 1 ; 1 9], x, [7 12 ; 9 14 ; 3 3 ; 1 1 ; 1 1 ; 4 9])",msgerr,999);
h5dataset(a, "D_set1", [10 10 ; 2 1 ; 4 1 ; 2 1 ; 1 9], x, [7 12 ; 9 14 ; 3 3 ; 1 1 ; 1 1 ; 4 9]);
assert_checkequal(a.root.D_set1.Data,z');

z = uint32(zeros(9,12));
z(1:5,1) = uint32([1;21;41;61;81]);
h5dataset(a, "D_set2", [5 20 ; 1 1 ; 1 1 ; 1 1 ; 5 1], y, [9 12 ; 9 14 ; 1 1 ; 1 1 ; 1 1 ; 5 1]);
assert_checkequal(a.root.D_set2.Data,z');

msgerr = msprintf(gettext("%s: %s\n") + "%s: %s\n", "h5dataset", gettext("Cannot write data in the dataset."), gettext("HDF5 description"), "src and dest data spaces have different sizes.");
assert_checkerror("h5dataset(a, ""Dset_3"", [10 10 ; 2 1 ; 4 1 ; 2 1 ; 1 9], x, [10 10 ; 11 14 ; 3 3 ; 1 1 ; 1 1 ; 1 1])",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5dataset", gettext("Invalid target dataspace."));
assert_checkerror("h5dataset(a, ""D_set4"", [10 10 ; 2 1 ; 4 1 ; 2 1 ; 1 9], x, [7 12 ; 6 11 ; 3 3 ; 1 1 ; 1 1 ; 4 9])",msgerr,999);

