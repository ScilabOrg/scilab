// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
//
// <-- Short Description -->
// White-box test for the getDiagramVersion macro.

loadScicosLibs();

// overload error
prot = funcprot();
funcprot(0);
function error(num, msg)
	global isErrorCall;
	isErrorCall = %t;
	abort;
endfunction
funcprot(prot);

global isErrorCall;
isErrorCall = %f;

getDiagramVersion();
if ~isErrorCall then pause, end
isErrorCall = %f;

getDiagramVersion([]);
if ~isErrorCall then pause, end
isErrorCall = %f;

// check that if the version number is filled, then the same value is returned.
scs_m = scicos_diagram();
scs_m.version = "customVersionName";
version = getDiagramVersion(scs_m);
if version <> scs_m.version then pause, end;

