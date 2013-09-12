// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) DIGITEO - 2010 - Michael Baudin
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x = sind(x)

    //Sine of x in degrees with exact values for integer multiples of 90

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"sind", 1));
    end
    
    if type(x)<>1 | ~isreal(x) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"sind",1));
    end

    n = round(x/90);
    x = x - n*90;
    m = pmodulo(n,4);
    x(m == 0) = sin(%pi/180 * x(m == 0));
    x(m == 1) = cos(%pi/180 * x(m == 1));
    x(m == 2) = -sin(%pi/180 * x(m == 2));
    x(m == 3) = -cos(%pi/180 * x(m == 3));
endfunction
