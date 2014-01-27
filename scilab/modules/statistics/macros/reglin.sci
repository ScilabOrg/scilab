// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [a,b,sig]=reglin(x,y,dflag)
    // Solves a linear regression
    // y=a(p,q)*x+b(p,1) + epsilon
    // x : matrix (q,n) and y matrix (p,n)
    // sig : noise standard deviation
    // dflag is optional if 1 a display of the result is done
    //!

    [lhs,rhs]=argn(0);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"reglin",2,3))
    end
    if rhs <=2;dflag=0;end
    [n1,n2]=size(x)
    [p1,p2]=size(y)
    if n2<>p2 then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"reglin",1,2));
    end;
    if size(y, "r") == 1 then
        // A column of x defines an element of y, but each line of y defines an independent problem, that is why we treat the y vector case separately here.
        // If x(:,i) contains %nan or y(i) = %nan, then both x(:,i) and y(i) are removed.
        nanX = isnan(x);
        nanY = isnan(y);
        if or(isnan(x)) | or(isnan(y)) then // At least one NaN is x or y.
            nan_indexes = [nanX; nanY];
            x(:, floor((find(nan_indexes==%t)-1)./(n1+1)+1)) = [];
            y(floor((find(nan_indexes==%t)-1)./(n1+1)+1)) = [];
            [n1, n2] = size(x);
            [p1, p2] = size(y);
        end
    elseif or(isnan(x)) | or(isnan(y)) then
        // This case is much more complicated if we don't want to lose any information. For now, just return NaNs.
        warning(msprintf(_("%s: NaNs detected in input arguments: NaNs will be returned.\n"), "reglin"))
    end

    xmoy=sum(x,2)/n2
    ymoy=sum(y,2)/n2
    // We use armax for appropriate orders which will perform
    // nothing but a least square
    // We could directly call pinv or \
    [arc,la,lb,sig]=armax(0,0,y-ymoy*ones(1,n2),x-xmoy*ones(1,n2),0,dflag);
    if typeof(la)=="list" then a=lb(1);else a=lb;end
    b=ymoy-a*xmoy;
endfunction
