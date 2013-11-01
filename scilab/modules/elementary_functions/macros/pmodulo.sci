// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2013 - Samuel GOUGEON :  : bugs 12373 & 13002
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function i = pmodulo(n, m)
    //i=pmodulo(n,m) the "positive modulo" of m et n.
    //i=n-floor(n./m).*m

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        msg = _("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "pmodulo", 2))
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
        msg = _("%s: Incompatible input arguments: Same types expected.\n")
        error(msprintf(msg, "pmodulo"))
    end

    // --------------------------  Processing ----------------------------

    if  type(n)==2 then
        [i,q] = pdiv(n0, m0);
    else
        m = abs(m)  // else returns i<0 for m<0 : http://bugzilla.scilab.org/12373
        if size(n0,"*") == 1 then
            i = n - floor(n ./ m) .* m
            k = find(i<0)   // this may occur for encoded integers
            i(k) = i(k) + m(k)
            if size(m0,"*")~=1 then
                i = matrix(i, size(m0))
            end

        elseif size(m0,"*") == 1 then
            i = n0 - floor(n0/m) * m
            k = find(i<0)   // this may occur for encoded integers
            i(k) = i(k) + m

        else
            if or(size(n0)<>size(m0)) then
                msg = _("%s: Wrong size for input arguments: Same size expected.\n")
                error(msprintf(msg, "pmodulo"))
            end
            i = n - floor(n ./ m) .* m
            k = find(i<0)   // this may occur for encoded integers
            i(k) = i(k) + m(k)
            i = matrix(i, size(m0))
        end
    end

endfunction
