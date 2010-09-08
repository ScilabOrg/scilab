// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
//
// <-- Short Description -->
// White-box test for the %diagram_xcos overload macro.

// overload called methods
prot = funcprot();
funcprot(0);
function xcos(h5path)
    global callWithPath;
    callWithPath = h5path;
endfunction
funcprot(prot);

// global declaration
global callWithPath;
callWithPath = "";

loadScicosLibs();

// test with an empty diagram
scs_m = scicos_diagram();
%diagram_xcos(scs_m);

scs_m_2 = scs_m;
status = import_from_hdf5(callWithPath);
if ~status | scs_m <> scs_m_2 then pause, end

// test with a diagram containing a block
scs_m = scicos_diagram();
scs_m.objs(1) = SUM_f("define");
%diagram_xcos(scs_m);

scs_m_2 = scs_m;
status = import_from_hdf5(callWithPath);
if ~status | scs_m <> scs_m_2 then pause, end

