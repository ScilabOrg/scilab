// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [P] = binomial(p,N)
    //
    //  PURPOSE
    //     Compute probabilities of the binomial law B(N,p)
    //
    //  PARAMETERS
    //     p : a real in [0,1]
    //     N : an integer >= 0
    //     P : a row vector with N+1 components and :
    //
    //        P(k+1) = P(X=k) = C(k,N) (1-p)^k p^(N-k)
    //
    //  Rewritten by Bruno for a gain in speed (by using
    //  cdfbin which computes the cumulative probability)
    //

    rhs = argn(2);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"binomial", 2));
    end

    if type(p) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"binomial",1));
    end

    if type(N) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer expected.\n"),"binomial",2));
    end

    if length(p) ~= 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real expected.\n"),"binomial",1));
    end

    if length(N) ~= 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"binomial",2));
    end

    if  p < 0  | p > 1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"binomial",1,0,1));
    end

    if  N < 1  |  floor(N)-N ~= 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Integer >= %d expected.\n"),"binomial",2,1));
    end

    un = ones(1,N+1);
    P = cdfbin("PQ", 0:N, N*un, p*un, (1-p)*un)
    P(2:N+1) = P(2:N+1) - P(1:N)
endfunction
