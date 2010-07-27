// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7078 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7078
//
// <-- Short Description -->
// A wrong error message is displayed when setting the 'value' property of a 'edit' uicontrol.

// Create a figure
h = gcf();
// Create an edit box
h_eb = uicontrol(h, "style", "edit", ...
	"position", [10 10 150 160], ...
	"string", "edit me", ...
	"max", 3, ...
	"min", 1);

ierr = execstr("set(h_eb, ""value"", ""pre-filled"");", "errcatch");

// Check we got the right error message
errMsgRef = msprintf(_("Wrong value for ''%s'' property: A String containing a numeric value expected.\n"), "Value");
if ierr==0 | lasterror()<>errMsgRef then pause; end

