// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 1990 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1990
//
// <-- Short Description -->
//   objfigure "position" initially undefined

f = figure(123);

if isempty(get(f,"position")) then pause; end


set(f,"position",[100 100 200 200]);

if or(get(f,"figure_position")<>[100 100]) then pause;end		
if or(get(f,"axes_size")<>[200 200]) then pause;end		
