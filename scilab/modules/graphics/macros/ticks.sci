// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - CNES - Alain Lamy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t] = ticks(locations, labels)
    // This function initializes a "ticks" structure in a graphical object
    // t = ticks(locations [, labels])
    // location: 1xN or Nx1 (row or column vector of real values)
    // labels: corresponding labels (same size as locations) or [].
    // if labels == [], a default format is used (%.8g).
    // if locations == [] => no ticks.
    // Examples:
    // a.x_ticks = ticks(1:10);
    // a.x_ticks = ticks(1:10, string(1:10));
    // a.x_ticks = ticks([]); // no ticks

    rhs = argn(2);

    if (rhs > 2 | rhs == 0)
        error("Invalid number of input arguments");
    end

    if (rhs == 1); labels = []; end

    // locations => must be real values
    if (typeof(locations) <> "constant")
        error("Invalid type for ''locations'' argument ");
    end

    // note: isreal([]) == %t, but in case ...
    if (~isreal(locations) & locations <> [])
        error("Invalid type for ''locations'' argument: real values expected");
    end

    // locations: row or column vector only (or [])
    if (min(size(locations)) > 1 | ndims(locations) > 2)
        error("Invalid size for ''locations'' argument: row or column vector expected");
    end

    if (size(labels, "*") > 0)
        // check "labels" type and size if it is <> []

        if (typeof(labels) <> "string")
            error("Invalid type for ''labels'' argument: string expected");
        end

        // check ndims to avoid problem with size
        if (ndims(locations) > 2)
            error("Invalid size for ''labels'' argument");
        end

        if (find(size(labels) <> size(locations)) <> [])
            error("Invalid size for ''labels'' argument");
        end

    else
        // labels == [] => default value for labels
        locations = matrix(locations, -1, 1); // column vector

        if (locations <> [])
            labels = msprintf("%.8g\n", locations);
        end

    end

    t = tlist(["ticks", "locations", "labels"], locations(:), labels(:));

endfunction
