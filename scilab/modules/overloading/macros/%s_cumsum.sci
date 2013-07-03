// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r = %s_cumsum(a, orient, typ)
    // call cumsum(a, orient, typ) only
    // orient must be a string or a scalar -> check type and size
    if and(type(orient)<> [1, 10]) then
        error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumsum",2))
    end

    if size(orient,"*")<>1 then
        if type(orient)==10 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum",2))
        else
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumsum",2))
        end
    end

    // If orient is a string, then orient must be equal to "m", "r", "c", or "*"
    // Else orient is an integer > 0
    if type(orient)==10 then
        if and(orient <> ["m","*","r","c"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
        end
    else
        if orient < 0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
        end
    end
    
    warning(msprintf(_("%s: The argument #%d is only used for matrices of integers or booleans.\n"), "cumsum", 3));
    r = cumsum(a, orient);
endfunction
