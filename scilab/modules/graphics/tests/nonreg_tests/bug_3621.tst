// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3621 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3621
//
// <-- Short Description -->
// Wrong setting of figure_position in the default figure.
//

defaultFig = gdf();
defaultPos = [200,300];

defaultFig.figure_position = defaultPos;

if (defaultFig.figure_position <> defaultPos) then pause; end;

newFig = scf();
if (newFig.figure_position <> defaultPos) then pause; end;

