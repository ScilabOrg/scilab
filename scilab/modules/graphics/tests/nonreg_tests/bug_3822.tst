// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3822 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3822
//
// <-- Short Description -->
// When a figure handle is saved and reloaded the figure_size as well as the viewport properties are not saved nor restored
// 

f = gcf();
f.auto_resize='off';
f.figure_size = [300 300];
f.viewport = [110 130];
save('foo.sav',"f");
delete(f)
load('foo.sav')
f = gcf();
if (f.figure_size <> [300 300]) then pause; end
if (f.viewport <> [110, 130]) then pause; end
