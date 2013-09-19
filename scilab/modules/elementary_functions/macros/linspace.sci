// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = linspace(d1, d2, n)
    // Linearly spaced vector.
    // linspace(x1, x2) generates a row vector of 100 linearly
    // equally spaced points between x1 and x2.
    // linspace(x1, x2, n) generates n points between x1 and x2.

    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"linspace", 2));
    end
    if size(d1,2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A column vector expected.\n"),"linspace",1));
    end
    if ~and(size(d1) == size(d2)) then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"linspace",1,2));
    end


    if rhs == 2 then
        n = 100;
    else
        if and(type(n)<>[1 8])  | size(n,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer value expected.\n"),"linspace",3));
        end

        if type(n) == 1 & int(n) <> n then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"),"linspace",3));
        elseif type(n) == 8 then
            n = double(n); // Convert for the operations to come
        end
    end

    if (n - 1) <= 0 then
        y = d2;
    else
        y = ((d2-d1) * (0:n-1)) / (n-1) + d1 * ones(1,n);

        // forces the last value to be in the interval
        y(:,$) = d2;
    end
endfunction
