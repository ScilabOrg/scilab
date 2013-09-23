// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - A. Khorshidi
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [res, barea, cc] = histcomp(n, data, yunit, cumulative)

    // Example:
    //     n = 10;
    //     data = rand(1, 1000, "normal");
    //     [class_freq, bin_area, class_center, class_boundary] = histcomp(n, data, yunit="percentage");
    //     plot(class_center, class_freq, "r-o") // A freq polygon

    rhs = argn(2);

    if rhs < 2
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "histplot", 2));
    end

    // Set default options
    if ~exists("cumulative", "local") then
        cumulative = %f;
    end
    if ~exists("yunit", "local") then
        yunit = "count";
    end

    nd = length(data); // Number of data values
    if length(n) == 1 then  // The number of classes is provided
        cb = linspace(min(data), max(data), n+1); // Class boundary
    else // The classes are provided
        cb = matrix(n,1,-1);   // force row form
    end
    cc = (cb(2:$)+cb(1:$-1))/2; // Class center
    cw = cb(2:$)-cb(1:$-1); // Bin width
    [ind, cf, info] = dsearch(data, cb); // cf: class frequency , ind: class number of each count

    // Frequency distribution
    select yunit
    case "count"
        cfreq = cf;
    case "normalized"
        cfreq = cf./cw/nd; // Normalized frequency
    case "relative"
        cfreq = cf/nd; // Relative frequency
    case "percentage"
        cfreq = cf/nd*100; // Percentage frequency
    end
    if cumulative then
        cfreq = cumsum(cfreq); // Cumulative frequency
    end
    barea = cw.*cfreq; // The area of each bin

    res.x = cb;
    res.y = cfreq;
    res.p = nd;

    //    tfreq = sum(cfreq); // Total freq
    //    tarea = sum(barea); // Total area

endfunction
