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

    if (rhs < 1 | rhs > 6) then
        error(msprintf(gettext("%s: Wrong number of input argument.\n"),"grand"))
    end

    if (rhs >= 4 & type(m) == 1 & type(n) == 1 & type(o) == 1) then
        // Three size arguments to grand(): run as if two arguments and resize at the end to obtain a hypermatix
        if (rhs == 4) then
            y = matrix(%_grand(m*n*o, 1, str), m, n, o);
            return
        end
        if (rhs == 5) then
            y = matrix(%_grand(m*n*o, 1, str, a), m, n, o);
            return
        end
        if (rhs == 6) then
            y = matrix(%_grand(m*n*o, 1, str, a, b), m, n, o);
            return
        end
        if (rhs == 7) then
            y = matrix(%_grand(m*n*o, 1, str, a, b, c), m, n, o);
            return
        end
    else
        // Only two size arguments, run grand() normally
        if (rhs == 3) then
            y = %_grand(m, n, o);
            return
        end
        if (rhs == 4) then
            y = %_grand(m, n, o, str);
            return
        end
        if (rhs == 5) then
            y = %_grand(m, n, o, str, a);
            return
        end
        if (rhs == 6) then
            y = %_grand(m, n, o, str, a, b);
            return
        end
        if (rhs == 6) then
            y = %_grand(m, n, o, str, a, b, c);
            return
        end

    end

endfunction
