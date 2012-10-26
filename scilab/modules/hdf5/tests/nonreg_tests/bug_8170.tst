// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

// <-- Non-regression test for bug 8170 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8170
//
// <-- Short Description -->
// Check format of HDF5 file
//

ierr = execstr("import_from_hdf5(SCI + ""/modules/hdf5/tests/nonreg_tests/bug_8170.sod"")", "errcatch");
if ierr == 0 then pause, end
