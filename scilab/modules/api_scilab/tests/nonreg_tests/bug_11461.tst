// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11461 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11461
//
// <-- Short Description -->
//  
//  api_list was not able to create lists and sub lists with undefined items

listRef = list();
listRef(1) = 1;
listRef(4) = list(1,2,3);
listRef(5) = 1;
myList = listRef;
export_to_hdf5(TMPDIR + "/test.sod", "myList");
clear myList;
import_from_hdf5(TMPDIR + "/test.sod");
assert_checkequal(listRef, myList);

