// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) - 2010 - DIGITEO - Michael Baudin
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function y = acscd(x)
    //Inverse cosecant, result in degrees.

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"acscd", 1));
    end

    if and(type(x) <> [1 5]) | ~isreal(x) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real sparse or full matrix expected.\n"),"acscd",1));
    end
    y = 180/%pi*asin(ones(x)./x);
endfunction
