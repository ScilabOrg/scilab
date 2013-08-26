//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

//
// This is the demonstration script of xstringb
// used when calling xstringb without any parameter.
//

function [] = %_xstringb()
    scf();
    axes = gca();
    axes.axes_visible = "on";
    str = ["Scilab","is";"not","Esilab"];
    xstringb(0.1,0.1,str,0.5,0.5,"fill");
    txt = gce();
    txt.box = "on";
endfunction
