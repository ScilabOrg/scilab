// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

// <-- Non-regression test for bug 7224 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7224
//
// <-- Short Description -->
// HDF5 functions don't unlock file wih empty matrix.
//

emp=[];         // or list([]), scicos_diagram(), etc.
save(TMPDIR + "/emp.sod","emp");
load(TMPDIR + "/emp.sod");
if execstr("save(TMPDIR + ""/emp.sod"",""emp"");","errcatch") then pause, end
