// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// Copyright (C) Scilab Enterprises - 2011 - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function hk = hank(m, n, cov)
    //<hk>=hank(m,n,cov)
    //this macro builds the hankel matrix of size (m*d,n*d)
    //from the covariance sequence of a vector process
    //   m  : number of bloc-rows
    //   n  : number of bloc-columns
    //   cov: sequence of covariances; it must be given as :[R0 R1 R2...Rk]
    //   hk : computed hankel matrix
    //

    [lhs, rhs] = argn(0);
    if rhs ~= 3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "hank", 3));
    end

    if m <= 0 | m ~= floor(m) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive integer expected.\n"), "hank", 1));
    end

    if n <= 0 | n ~= floor(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive integer expected.\n"), "hank", 2));
    end

    t = type(cov);
    if t ~= 1 & t ~= 2 & t ~= 4 & t ~= 5 & t ~= 6 & t ~= 7 & t ~= 8 & t ~= 10 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Unsupported ''%s'' type.\n"), "hank", 3, typeof(t)));
    end

    [r, c] = size(cov);
    mr = m * r;
    nr = n * r;
    if mr + nr - r > c then
        error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"), "hank"));
    end

    index = ones(1, nr) .*. (1:r:mr)' + (0:(nr - 1)) .*. ones(m, 1);
    hk = matrix(cov(:, index), mr, -1);
endfunction
