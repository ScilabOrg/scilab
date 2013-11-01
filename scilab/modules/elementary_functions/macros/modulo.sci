// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - Samuel GOUGEON : Bug 13002 : extension to hypermatrices & integers
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function i = modulo(n, m)
    //i=modulo(n,m) returns  n modulo m.

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "modulo", 2))
    end

    m0 = m
    n0 = n
    m = m(:)
    n = n(:)

    // -----------------------  Checking arguments --------------------------

    if and(type(n) <> [1 2 8]) | (typeof(n)=="constant" & ~isreal(n)) then
        msg = _("%s: Wrong type for input argument #%d: A real expected.\n")
        error(msprintf(msg, "modulo", 1))
    end

    if and(type(m) <> [1 2 8]) | (typeof(m)=="constant" & ~isreal(m)) then
        msg = _("%s: Wrong type for input argument #%d: A real expected.\n")
        error(msprintf(msg, "modulo", 2))
    end

    if type(n)==8 & type(n)~=type(m) then
        msg = _("%s: Incompatible input arguments #%d and #%d: Same types expected.\n")
        error(msprintf(msg, "pmodulo", 1, 2))
    end

    // --------------------------  Processing ----------------------------

    if or(type(m)==[1 8]) & type(m)==type(n) then
        if size(n0,"*")==1 | size(m0,"*")==1 then
            i = n0 - int(n0 ./ m) .* m
            if size(m0,"*")~=1 then
                i = matrix(i, size(m0))
            end
        else
            if or(size(n0) <> size(m0)) then
                msg = _("%s: Wrong size for input arguments: Same size expected.\n")
                error(msprintf(msg, "modulo"))
            end
            i = n - int(n ./ m) .* m
            i = matrix(i, size(m0))
        end
    else
        [i,q] = pdiv(n0, m0)
    end

endfunction






