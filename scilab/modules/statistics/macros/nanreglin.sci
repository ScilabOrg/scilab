// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [a, b, sig] = nanreglin(x, y, dflag)
    // Solves a linear regression
    // y=a(p,q)*x+b(p,1) + epsilon  where y or x contain NaNs.
    // x : matrix (q,n) and y matrix (p,n)
    // dflag is optional if 1 a display of the result is done
    //!

    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanreglin",2,3))
    end
    if rhs <= 2 then
        dflag = 0;
    end
    [n1, n2] = size(x);
    [p1, p2] = size(y);
    if n2 <> p2 then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"nanreglin",1,2));
    end
    if size(y, "r") == 1 then
        // reglin can already handle NaNs if y is not a matrix.
        [a, b] = reglin(x, y, dflag);
        return
    end
    if lhs > 2 then
        // When y is a matrix containing NaNs, the standard deviation requires to solve a badly-dimensioned problem.
        error(msprintf(_("%s: Cannot output standard deviation when y is a matrix containing NaNs. Wrong number of output arguments: %d to %d expected.\n"),"nanreglin",1,2))
    end

    a = zeros(p1, n1);
    b = zeros(p1, 1);
    for i=1:p1
        y2 = y(i, find(~isnan(y(i,:))));
        x2 = x(:, find(~isnan(y(i,:))));
        [a(i, :), b(i)] = reglin(x2, y2, dflag);
    end

endfunction
