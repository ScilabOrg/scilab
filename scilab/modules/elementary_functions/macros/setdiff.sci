// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [a, ka] = setdiff(a, b)
    // returns a values which are not in b

    rhs = argn(2);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "setdiff", 2));
    end

    if b == [] then //b is empty reurn a!
        ka = 1:size(a,"*");
        return
    end

    [a,ka] = unique(a);
    na = size(a,"*");

    b = unique(b(:));

    if type(a) == 10 then
        [x,k] = gsort([a(:); b], "g", "i");
    else
        [x,k] = gsort([a(:); b], "g", "d");
    end
    if type(a) == 1 then x=x($:-1:1);k=k($:-1:1);,end
    d = find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
    if d <> [] then
        k([d;d+1]) = [];
    end

    keep = find(k <= na);
    a = a(k(keep));
    ka = ka(k(keep));
endfunction
