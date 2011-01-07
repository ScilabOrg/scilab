*// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8759 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8759
//
// <-- Short Description -->
// The callback of "listbox" style uicontrols was executed many times instead of once when an item was selected.

h = scf();

l = uicontrol("Parent", h, ..
    "Position", [10 10 200 200], ..
    "Style", "listbox", ..
    "String", ["item1", "item2", "item3", "item4"], ..
    "Callback", "set(l, ""Enable"", ""off"");disp(""callback"");set(l, ""Enable"", ""on"");");

// Select each item alternatively and check that "callback" is displayed only once per selection