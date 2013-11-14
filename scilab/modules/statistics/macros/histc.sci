// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - A. Khorshidi
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [cf, ind] = histc(n, data, normalization)

    // Example:
    //     n = 10;
    //     data = rand(1, 1000, "normal");
    //     [class_freq, ind] = histc(n, data)

    rhs = argn(2);

    if rhs < 2
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "histc", 2));
    end

    if rhs == 3 then
        if type(normalization) ~= 4 | size(normalization, "*") ~= 1 then
            error(msprintf(gettext("%s: Wrong type of input argument #%d: A single boolean expected.\n"), "histc", 3));
        end
    else
        // By default, normalize the result
        normalization = %t
    end

    if n < 1
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"histc",1,"[1, oo)"));
    elseif n ~= floor(n)
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Integer expected.\n"),"histc",1))
    end

    nd = length(data); // Number of data values
    if length(n) == 1 then  // The number of classes is provided
        cb = linspace(min(data), max(data), n+1); // Class boundary
    else // The classes are provided
        cb = matrix(n, 1, -1);   // force row form
        if min(diff(cb)) <= 0 then
            error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"),"histc",1))
        end
    end

    [ind, cf, info] = dsearch(data, cb); // cf: class frequency, ind: class number of each count

    // Normalization
    if normalization == %t then
        cw = cb(2:$)-cb(1:$-1); // Bin width
        cf = cf./(nd*cw);
    end

endfunction
