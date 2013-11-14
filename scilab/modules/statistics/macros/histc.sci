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
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "histplot", 2));
    end

    if ~exists("normalization","local") then
        normalization = %t
    end

    nd = length(data); // Number of data values
    if length(n) == 1 then  // The number of classes is provided
        cb = linspace(min(data), max(data), n+1); // Class boundary
    else // The classes are provided
        cb = matrix(n,1,-1);   // force row form
    end
    cw = cb(2:$)-cb(1:$-1); // Bin width

    [ind, cf, info] = dsearch(data, cb); // cf: class frequency, ind: class number of each count

    // Normalization
    if normalization == %t then
        cf = cf./(nd*cw);
    end

endfunction
