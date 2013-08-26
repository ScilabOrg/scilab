// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function varargout = ind2sub(dims, I)
    //  ind2sub is used to determine the equivalent subscript values
    //   corresponding to a given single index into an array.
    //
    //   [i1,i2,..] = ind2sub(dims,I) returns the arrays i1, i2, ... containing the
    //   equivalent row, column, ... subscripts corresponding to the index
    //   matrix I for a matrix of size dims.
    //
    //   Mi=ind2sub(dims,I) returns a matrix Mi whose columns are the arrays i1(:), i2(:), ...

    rhs = argn(2);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "ind2sub", 2));
    end

    varargout = list();
    d = cumprod(double(dims));
    k1 = I;
    k = [];
    k1 = k1 - 1;

    for i = size(d, "*")-1:-1:1
        q = floor(k1/d(i));
        k1 = k1 - q * d(i);
        varargout(0) = q + 1;
    end

    varargout(0) = k1 + 1;
    if argn(1) == 1 then //return a matrix whose columns are the subindices
        K = []
        for k = varargout
            K = [K matrix(k, -1, 1)];
        end
        varargout = list(K);
    end
endfunction
