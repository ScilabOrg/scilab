// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9510 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9510
//
// <-- Short Description -->
// export does not return %f when try to export unmanaged data types.
//


deff('y=f(x)','y=2*x');
if export_to_hdf5(TMPDIR+"/functions.sod","f") <> %f then pause end
if ls(TMPDIR+"/functions.sod") <> [] then pause end
