// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2013 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function retval = unwrap(a,tol,dim)

    [lhs,rhs]=argn(0);

    if (rhs <> 3) then
        if (rhs == 2) then
            if (size(a,"r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        elseif (rhs == 1) then
            tol = %pi;
            if (size(a,"r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        else
            error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "unwrap", 1));
        end
    end

    if (tol == []) then
        tol = %pi;
    end
    tol = abs(tol);

    rng = 2*%pi;

    if (dim == 1)
        ra = a.';
    else
        ra = a;
    end
    [m,n] = size(ra);

    if (m == 1)
        retval = a;
        return;
    end

    d = [zeros(1,n); ra(1:m-1,:)-ra(2:m,:)];
    for i=1:m
        for j=1:n
            if (d(i,j) < -tol) then
                d(i,j) = -rng;
            else
                if (d(i,j) > tol) then
                    d(i,j) = rng;
                else
                    d(i,j) = 0;
                end
            end
        end
    end

    r = cumsum(d);

    if (dim == 1) then
        retval = (ra + r).';
    else
        retval = (ra + r);
    end

endfunction
