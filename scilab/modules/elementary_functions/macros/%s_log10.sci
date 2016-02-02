// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = %s_log10(x)
    [lhs, rhs] = argn(0);
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"log10", 1));
    end
    y = log(x) / log(10);
endfunction
