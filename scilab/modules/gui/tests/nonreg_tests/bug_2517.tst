// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2517 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2517
//
// <-- Short Description -->
//     "position" property format "x|y|w|h" not accepted by figure(), despite what said in help

f = figure(1,"position","100|100|300|500");

if or(get(f,"figure_position")<>[100 100]) then pause;end		
if or(get(f,"axes_size")<>[300 500]) then pause;end		
