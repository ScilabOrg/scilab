// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2013 - Allan CORNET
// Copyright (C) - 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function retval = unwrap(a, tol, dim)

    [lhs, rhs]=argn(0);

    if (rhs <> 3) then
        if (rhs == 2) then
            if (size(a, "r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        elseif (rhs == 1) then
            tol = %pi;
            if (size(a, "r") == 1) then
                dim = 1;
            else
                dim = 2;
            end
        elseif (rhs == 0) then
            %_unwrap()   // Display an example
            retval = []
            return
        else
            error(msprintf(gettext("%s: Wrong number of input argument(s): at maximum %d expected.\n"), "unwrap", 3));
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

    d = [zeros(1, n); ra(1:m-1,:)-ra(2:m,:)];
    d(abs(d)<=tol) = 0
    k = (d~=0)
    d(k) = sign(d(k)).*rng

    r = cumsum(d);

    if (dim == 1) then
        retval = (ra + r).';
    else
        retval = (ra + r);
    end

endfunction
