// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8818 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8818
//
// <-- Short Description -->
// Modelica exponential format was wrong.
//

scs_m = MBLOCK("define");

// default string value (scilab notation)
exp_value = string(17D63);

// setting parameter value (L)
scs_m.graphics.exprs(7)($) = exp_value;

fprot = funcprot();
funcprot(0);
function %str = x_mdialog(%desc,%labels,%ini)
	%str = %ini;
endfunction
function result = x_dialog(labels, valueini)
	result = valueini;
endfunction
funcprot(fprot);

scs_m = MBLOCK("set", scs_m);

if ~isempty(strstr(scs_m.graphics.exprs($), exp_value)) then pause, end

