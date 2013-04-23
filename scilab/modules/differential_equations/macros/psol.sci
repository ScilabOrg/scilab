// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [r, ier] = psol(wp, iwp, b)
    ier = 0;
    //Compute the LU factorization of R.
    sp = sparse(iwp, wp);
    [h, rk] = lufact(sp);
    //Solve the system LU*X = b
    r = lusolve(h, b);
    ludel(h);
endfunction
