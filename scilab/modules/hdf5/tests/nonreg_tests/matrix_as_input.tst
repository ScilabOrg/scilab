// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Short Description -->
// If the input of import_from_hdf5 was a matrix of string, a segfault occurred
//

myFiles=ls(SCI+"/modules/types/tests/java/org/scilab/tests/modules/types/referenceData/*");
// Used to segfault
assert_checkerror("import_from_hdf5(myFiles)",[],999);
