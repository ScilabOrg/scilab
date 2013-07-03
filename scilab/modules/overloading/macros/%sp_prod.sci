// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%sp_prod(a,d,typ)
    rhs = argn(2);
    dims=size(a);
    if rhs==1 then
        d= 0 //"*"
    else
        // call prod(a,d) or prod(a, d, typ)
        // d must be a string or scalar -> check type and siz
        if and(type(d) <> [1 10]) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"prod",2))
        end

        if size(d,"*")<>1 then
            if type(d)==10 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",2))
            else
                error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
            end
        end

        // call prod(a, d) with d = "native" or "double"
        if rhs == 2 & or(d==["native","double"]) then
            d=0 //"*"
        else
            // If d is a string, d= "m", "r", "c", "*"
            // Else d is an integer > 0
            if type(d)==10 then
                d=find(d==["m","*","r","c"])
                if d==[] then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
                d=d-2
            else
                if d<0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
            end

            // call prod(a, d, typ) but typ is omitted
            if rhs == 3 then
                warning(msprintf(_("%s: The argument #%d is only used for matrices of integers or booleans.\n"), "prod", 3));
            end
        end
    end

    if d==-1 then //'m'
        d=find(dims>1,1)
        if d==[] then d=0,end
    end

    select d
    case 0 then
        r=prod(a)
    case 1 then
        for k=1:dims(2)
            r(1,k)=prod(a(:,k))
        end
        r=sparse(r)
    case 2 then
        for k=1:dims(1)
            r(k,1)=prod(a(k,:))
        end
        r=sparse(r)
    else
        r=a
    end

endfunction
