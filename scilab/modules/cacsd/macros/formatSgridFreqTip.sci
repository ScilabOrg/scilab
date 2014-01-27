// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function str = formatSgridFreqTip(datatipHandle)
    //This function is called by the datatip mechanism to format the tip
    //string for the sgrid chart iso natural frequency curves.
    str = msprintf("%.2g"+_("rad/sec"),datatipHandle.parent.display_function_data);
endfunction
