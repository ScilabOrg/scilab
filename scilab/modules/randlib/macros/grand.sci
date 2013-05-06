// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function y = grand(m, n, o, str, a, b, c)

    [lhs, rhs]=argn(0);

    if (rhs < 1 | rhs > 7) then
        error(msprintf(gettext("%s: Wrong number of input arguments.\n"),"grand"))
    end

    y = [] // Default return

    if (rhs >= 4 & type(m) == 1 & type(n) == 1 & type(o) == 1) then
        // grand(m, n, o, 'string'). Run as if two arguments and resize at the end to obtain a hypermatix
        select rhs,
        case 4 then
            y = matrix(%_grand(m*n*o, 1, str), m, n, o), return,
        case 5 then
            y = matrix(%_grand(m*n*o, 1, str, a), m, n, o), return,
        case 6 then
            y = matrix(%_grand(m*n*o, 1, str, a, b), m, n, o), return,
        else // case 7
            y = matrix(%_grand(m*n*o, 1, str, a, b, c), m, n, o), return,
        end
    else
        // grand(m, n, 'string'). Run grand() normally
        select rhs,
        case 1 then
            y = %_grand(m), return,
        case 2 then
            %_grand(m, n), return,
        case 3 then // Can be grand('string', i, j), so have to test first argument.
            if (type(m) == 1) then
                y = %_grand(m, n, o), return,
            else
                %_grand(m, n, o), return,
            end
        case 4 then
            y = %_grand(m, n, o, str), return,
        case 5 then // Can be grand('string', i, j, k, l), so have to test first argument.
            if (type(m) == 1) then
                y = %_grand(m, n, o, str, a), return,
            else
                %_grand(m, n, o, str, a), return,
            end
        case 6 then
            y = %_grand(m, n, o, str, a, b), return,
        else // 7 input arguments but the fist three are not integers
            error(msprintf(gettext("%s: Wrong type of input arguments.\n"),"grand"))
        end
    end

endfunction
