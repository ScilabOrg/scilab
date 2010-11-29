// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8480 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8480
//
// <-- Short Description -->
// The debug block did not have a valid function type.
//

// start a clean scilab
// launch Xcos
xcos
// Drag'n'drop the DEBUG_SCICOS block (from User-defined functions)
// Right click on this block and open the block details
//    Check that Root.model.sim(1) == "%debug_scicos"
//    Check that Root.model.sim(2) == 99

