// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enteprises - Paul Bignier: added mean squared deviation
//                                                        (third input argument)
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function sd = stdev(x, o, m)
    //
    //This function computes  the  standard deviation  of  the values  of  a
    //vector or matrix x.
    //
    //For a vector or a  matrix x, s=stdev(x)  returns in  the scalar s  the
    //standard deviation of all the entries of x.
    //
    //s=stdev(x,'r')  (or,  equivalently,   s=stdev(x,1))   is the   rowwise
    //standard deviation. It returns  in each entry of the  row vector s the
    //standard deviation of each column of x.
    //
    //s=stdev(x,'c')  (or,  equivalently, s=stdev(x,2))  is   the columnwise
    //standard  deviation. It returns in each  entry of the  column vector y
    //the standard deviation of each row of x.
    //
    //The input argument m represents the a priori mean. If it is present, then the sum is
    //divided by size(x,"*"). Otherwise ("sample standard deviation"), it is divided by size(x,"*")-1.
    //

    [lhs, rhs] = argn(0);

    if and(typeof(x) ~= ["constant" "hypermat"]) | ~isreal(x) then
        tmp = _("%s: Wrong type for input argument #%d: A real matrix expected.\n")
        error(msprintf(tmp, "stdev", 1))
    end

    siz = size(x);
    if rhs == 3 then
        if and(typeof(m) ~= ["constant" "hypermat"]) | ~isreal(m) then
            tmp = _("%s: Wrong type for input argument #%d: A real matrix expected.\n")
            error(msprintf(tmp, "stdev", 3))
        elseif o == "*" then
            if ~isscalar(m) then
                tmp = _("%s: Wrong size for input argument #%d.\n")
                error(msprintf(tmp, "stdev", 3))
            end
        elseif o == "r" | o == 1 then
            if size(m)~=[1 siz(2:$)] & ~isscalar(m) then
                tmp = _("%s: Wrong size for input argument #%d.\n")
                error(msprintf(tmp, "stdev", 3))
            end
        elseif o == "c" | o == 2 then
            if size(m)~=[siz(1) 1 siz(3:$)] & ~isscalar(m) then
                tmp = _("%s: Wrong size for input argument #%d.\n")
                error(msprintf(tmp, "stdev", 3))
            end
        end
    end
    if rhs < 2 then
        o = "*";
    end

    if x == [] then
        sd = %nan;
        return
    end

    if typeof(x) == "hypermat" then
        if rhs == 3 then
            sd = %hm_stdev(x, o, m);
        else
            sd = %hm_stdev(x, o);
        end
        return
    end

    // Remove the mean
    if o == "*" then
        if rhs == 3
            y = x - m;
        else
            y = x - mean(x);
        end
    elseif o == "c" | o == 2 then
        if rhs == 3
            y = x - m*ones(x(1,:));
        else
            y = x - mean(x,"c")*ones(x(1,:));
        end
    elseif o == "r" | o == 1 then
        if rhs == 3
            y = x - ones(x(:,1))*m;
        else
            y = x - ones(x(:,1))*mean(x,"r");
        end
    elseif type(o) == 1 & o == floor(o) & o > ndims(x) then
        sd = 0*x;
        return
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'' or a positive integer expected.\n"),"stdev",2,"*","r","c")),
    end

    mn = size(x, o);

    if mn == 1 then
        sd = 0*y;
    else
        if rhs == 2 then
            sd = sqrt(sum(y.^2,o)/(mn-1));
        else
            sd = sqrt(sum(y.^2,o)/mn);
        end
    end

endfunction
