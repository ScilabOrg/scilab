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

function sd = stdev(x, o, ms)
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
    //s=stdev(x,orien,%t)  returns the mean squared deviation of x, that is,
    //normalized by N.
    //

    if argn(2) < 3 then
        ms = %f
    end
    if argn(2) == 3 & and(ms <> [%f %t]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"stdev",3,"%f","%t"));
    end
    if argn(2) < 2 then
        o = "*";
    end

    if x == [] then
        sd = %nan;
        return
    end

    if typeof(x) == "hypermat" then
        sd = %hm_stdev(x, o, ms);
        return
    end

    // Remove the mean
    if o == "*" then
        y = x - mean(x);
    elseif o == "c" | o == 2 then
        y = x - mean(x,"c")*ones(x(1,:));
    elseif o == "r" | o == 1 then
        y = x - ones(x(:,1))*mean(x,"r");
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"),"stdev",2,"*","c","r",1,2)),
    end

    mn = size(x, o);

    if mn == 1 then
        sd = 0*y;
    else
        if ms == %f then
            sd = sqrt(sum(y.^2,o)/(mn-1));
        else
            sd = sqrt(sum(y.^2,o)/mn);
        end
    end

endfunction
