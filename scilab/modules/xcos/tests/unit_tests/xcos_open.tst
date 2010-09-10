// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- TEST WITH XCOS -->
//
// <-- Short Description -->
// White-box test for the xcos_open macro.

// overload called methods
prot = funcprot();
funcprot(0);
function xcosDiagramOpen(uid, showed)
    global callWithUID;
    global callWithShowed;
    
    [callWithUID, callWithShowed] = (uid, showed);
endfunction
funcprot(prot);

// global definitions
global callWithUID;
global callWithShowed;
callWithUID = "";
callWithShowed = %f;

loadScicosLibs();
scs_m = scicos_diagram();

scs_m.objs(1) = SUM_f("define");
scs_m.objs(2) = BIGSOM_f("define");
scs_m.objs(2).doc = list("id2");
scs_m.objs(3) = PRODUCT("define");
scs_m.objs(3).doc = list("id3", "garbage1", "garbage2");

// test without doc
xcos_open(1);
if callWithUID <> "" | callWithShowed then pause, end

// test without doc but showed
xcos_open(1, %t);
if callWithUID <> "" | callWithShowed then pause, end

// test with doc
xcos_open(2);
if callWithUID <> "id2" | ~callWithShowed then pause, end
callWithUID = "";
callWithShowed = %f;

// test with doc but showed
xcos_open(2, %t);
if callWithUID <> "id2" | ~callWithShowed then pause, end
callWithUID = "";
callWithShowed = %f;

// test with doc and garbage
xcos_open(3);
if callWithUID <> "id3" | ~callWithShowed then pause, end

