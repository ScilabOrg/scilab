// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// This macro is called in sci_grand.c when the three first input arguments
// are sci_matrix (m, n and o, meaning that the user seeks a hypermatrix).
//
// Its purpose is to call y = grand(m*n*o, 1, ...)
// and to apply matrix(y, m, n, o), in order to obtain the sought hypermatrix.

function y = %_Rand(m, n, o, str, a, b, c)

    [lhs, rhs] = argn(0);

    select rhs,
    case 4 then
        y = matrix(grand(m*n*o, 1, str), m, n, o), return,
    case 5 then
        y = matrix(grand(m*n*o, 1, str, a), m, n, o), return,
    case 6 then
        y = matrix(grand(m*n*o, 1, str, a, b), m, n, o), return,
    else // case 7
        y = matrix(grand(m*n*o, 1, str, a, b, c), m, n, o), return,
    end

endfunction
