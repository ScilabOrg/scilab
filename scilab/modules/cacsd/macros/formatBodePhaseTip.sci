// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  1985-2010 - INRIA - Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function str = formatBodePhaseTip(datatipHandle)
    // This function is called by the datatip mechanism to format the tip
    // string for the bode phase curves
    pt = datatipHandle.data(1:2)
    str = msprintf("%.4g"+_("Hz")+"\n %.4g"+"°", pt(1), pt(2));
endfunction
