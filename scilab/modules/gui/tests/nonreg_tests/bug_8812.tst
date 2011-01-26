// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8812 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8812
//
// <-- Short Description -->
// "Value" property of a "Listbox" style uicontrol did not always contain the selected items indexes.

f = figure("Position", [30,200,320,300])
delmenu(f.figure_id, "File")
delmenu(f.figure_id, "Tools")
delmenu(f.figure_id, "Edit")
delmenu(f.figure_id, "?")

uicontrol("Parent", f, ..
    "Style", "listbox", ..
    "Position", [20, 120, 280, 170], ..
    "Backgroundcolor", [1 1 1], ..
    "String",  ["sin(t)" "cos(t)" "tan(t)" "exp^-t" "x^3" "x" "-x" "x^2"], ..
    "ForegroundColor", [0.55 0.27 0.07], ..
    "Callback", "disp(get(gcbo, ""Value""))");

// Check that the callback is executed just one time for each selection change.
// Check that the displayed values are the selected items indexes.

