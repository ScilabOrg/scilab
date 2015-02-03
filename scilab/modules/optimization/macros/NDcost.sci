// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [f, g, ind] = NDcost(x, ind, fun, varargin)
    // external for optim()
    // Computes the gradient of 'fun' at 'x' using code differentiation
    if argn(2) < 4 then
        f = fun(x);
        g = numderivative(fun, x, %eps^(1/3));
    else
        f = fun(x, varargin(:));
        g = numderivative(list(fun, varargin(:)), x, %eps^(1/3));
    end
endfunction
