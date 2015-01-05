// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = %s_log10(x)
    [lhs, rhs] = argn(0);
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"log10", 1));
    end
    y = log(x) / log(10);
endfunction
