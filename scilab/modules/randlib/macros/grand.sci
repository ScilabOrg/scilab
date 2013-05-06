// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function y = grand(m, n, o, mode, a, b, c)

    [lhs, rhs]=argn(0);

    if (rhs >= 3 & type(m) == 1 & type(n) == 1 & type(o) == 1) then
        // Three size arguments to grand(): run as if two arguments and resize at the end to obtain a hypermatix
        y = matrix(%_grand(m*n, 1, mode, a, b, c), m, n, o);
    else
        // Only two size arguments, run grand() normally
        y = %_grand(m, n, o, mode, a, b, c);
    end

endfunction
