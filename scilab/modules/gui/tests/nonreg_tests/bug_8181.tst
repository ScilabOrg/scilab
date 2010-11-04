// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8181 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8181
//
// <-- Short Description -->
// uicontrol did not handle color where composants are outside the range 0-255.

h = uicontrol("Style", "text");
h.BackgroundColor = [%inf -12 %nan];

// No critical error
