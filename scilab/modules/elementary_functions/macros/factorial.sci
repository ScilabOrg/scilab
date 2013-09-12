// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier: added integer support and gamma()
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = factorial(n)

    // This function returns the factorial n. If n is a vector, matrix or hypermatrix it returns the element wise factorial
    // Input  : n, a scalar/vector/matrix/hypermat of positives integers.
    // Output : y, a scalar/vector/matrix/hypermat of same type as n.

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "factorial", 1));
    end

    if ((type(n) <> 8 & type(n) <> 1) & (typeof(n)<>"hypermat" | (type(n.entries)<>8 & type(n.entries)<>1))) | (~isempty(n) & (or((n-floor(n)<>0)) | or(n<0))) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix/hypermatrix of positive integers expected.\n"), "factorial", 1));
    elseif isempty(n)
        y = n;
        return
    else
        // Stop now in case of integer type limitation
        msg = _("%s: Wrong value for input argument #%d: Result will exceed type limitation");
        if type(n) == 8 then
            if (or(typeof(n) == ["int8" "uint8"]) & n > 5)  | ..
                (typeof(n) == "int16" & n > 7)                | ..
                (typeof(n) == "uint16" & n > 8)               | ..
                (or(typeof(n) == ["int32" "uint32"]) & n > 12) then
                error(msprintf(msg, "factorial", 1));
            end
        end
        // Convert data for gamma(). Integer hypermatrices are handled by cumprod()
        converted = 0;
        if type(n) == 8 & typeof(n) <> "hypermat" then
            converted = 1;
            typeN = inttype(n);
            n = double(n);
        end
        // Actual work
        if typeof(n) == "hypermat" then
            n(n==0) = 1;
            ntemp = cumprod(1:max(n));
            y = matrix(ntemp(n(:)), size(n));
        else
            y = gamma(n+1);
        end
        // Convert result back
        if converted == 1 then
            y = iconvert(y, typeN);
        end
    end

endfunction
