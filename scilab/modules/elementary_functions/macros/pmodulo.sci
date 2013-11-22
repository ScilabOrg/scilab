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

    mt = type(m($))
    nt = type(n($))

    // -----------------------  Checking arguments --------------------------

    if or(type(n)==[15 16]) | and(nt <> [1 2 8]) | (nt==1 & ~isreal(n)) then
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "pmodulo", 1))
    end

    if or(type(m)==[15 16]) | and(mt <> [1 2 8]) | (mt==1 & ~isreal(m)) then
        msg = _("%s: Wrong type for input argument #%d: Real, integer or polynomial matrix expected.\n")
        error(msprintf(msg, "pmodulo", 2))
    end

    if (nt==8 | mt==8)  & nt~=mt
        msg = _("%s: Incompatible input arguments: Same types expected.\n")
        error(msprintf(msg, "pmodulo"))
    end

    // --------------------------  Processing ----------------------------

    if  nt==2 then
        [i,q] = pdiv(n, m)
    else
        ms = size(m)
        ns = size(n)
        m = m(:)
        n = n(:)
        m = abs(m)  // else returns i<0 for m<0 : http://bugzilla.scilab.org/12373
        if length(n)>1 & length(m)>1 & or(ns<>ms) then
            msg = _("%s: Wrong size for input arguments: Same size expected.\n")
            error(msprintf(msg, "pmodulo"))
        end
        i = n - floor(n ./ m) .* m
        k = find(i<0)           // this may occur for encoded integers
        if length(m)>1 then
            i(k) = i(k) + m(k)
            i = iconvert(i, inttype(n))
            i = matrix(i, ms)
        else
            i(k) = i(k) + m
            i = iconvert(i, inttype(n))
            i = matrix(i, ns)
        end
    end

endfunction
