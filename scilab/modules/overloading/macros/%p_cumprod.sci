// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%p_cumprod(a,d,typ)
    rhs = argn(2);
    if rhs==1 then
        typ=list()
        d=0 //"*"
    else
        // call cumprod(a, d) or cumprod(a, d, typ)
        // d must be a string or a scalar -> check type and size
        if and(type(d)<> [1, 10]) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumprod",2))
        end

        if size(d,"*")<>1 then
            if type(d)==10 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",2))
            else
                error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod",2))
            end
        end

        // call cumprod(a, d) with d = "native" or "double"
        if rhs==2 & or(d==["native","double"]) then
            typ=list(d)
            d=0 //"*"
        else
            // If d is a string, d = "m", "r", "c" or "*"
            // Else d is an integer > 0
            if type(d)==10 then
                d=find(d==["m","*","r","c"])
                if d==[] then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
                d=d-2
            else
                if d<0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
            end
            if rhs == 3 then
                warning(msprintf(_("%s: The argument #%d is only used for matrices of integers or booleans.\n"), "cumprod", 3));
            end
            typ=list()
        end
    end

    dims=size(a);

    if d==-1 then //'m'
        d=find(dims>1,1)
        if d==[] then d=0,end
    end

    select d
    case 0 then
        for k=2:prod(dims)
            a(k)=a(k-1)*a(k)
        end
    case 1 then
        for k=1:dims(2)
            a(:,k)=cumprod(a(:,k),typ(:))
        end
    case 2 then
        for k=1:dims(1)
            a(k,:)=cumprod(a(k,:),typ(:))
        end
    else
        a=a
    end
endfunction
