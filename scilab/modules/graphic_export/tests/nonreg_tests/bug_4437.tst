// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2009 - DIGITEO - Jean-Baptiste SILVY
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4437 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4437
//
// <-- Short Description -->
// Exporting an empty figure crashes Scilab
//

figure(0);
xs2eps(0, TMPDIR + "name");

// should not crash


