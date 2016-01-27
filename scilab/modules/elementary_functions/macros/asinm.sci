// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

function t=asinm(x)
    //Matrix wise Sine-inverse of x

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"asinm",1));
    end

    if type(x)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"asinm",1));
    end

    [m,n]=size(x);

    if m<>n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"asinm",1));
    end

    t=-%i*logm(%i*x+sqrtm(eye()-x*x));

endfunction
