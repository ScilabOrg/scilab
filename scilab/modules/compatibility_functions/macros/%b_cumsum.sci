// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%b_cumsum(varargin)
    a=varargin(1)
    if varargin($)=="native" then
        // call cumsum(a, d) with d = "native"
        if size(varargin)==2 then
            d = 0 //"*"
            // call cumsum(a, d, typ) with typ = "native"
        elseif size(varargin)==3 then
            d=varargin(2)

            // d must be a string or a scalar -> check type and size
            if and(type(d)<> [1, 10]) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumsum",2))
            end

            if size(d,"*")<>1 then
                if type(d)==10 then
                    error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum",2))
                else
                    error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumsum",2))
                end
            end

            // call cumsum(a, d, typ) with d = "m", "r", "*" or "c"
            // if it's not this case, d can be an integer > 0
            // else ERROR
            if type(d)==10 then
                d=find(d==["m","*","r","c"])
                if d==[] then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
                d=d-2
            else
                if d<0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
            end
        end

        dims=size(a);

        if d==-1 then //'m'
            d=find(dims>1,1)
            if d==[] then d=0,end
        end

        select d
        case 0 then
            k=find(a,1)
            if k<>[] then a(k:$)=%t,end
        case 1 then
            for k=1:dims(2)
                a(:,k)=cumsum(a(:,k),"native")
            end
        case 2 then
            for k=1:dims(1)
                a(k,:)=cumsum(a(k,:),"native")
            end
        else
            a=a
        end
    else
        // converts boolean to double
        varargin(1)=bool2s(varargin(1))

        // If rhs = 3, check d and typ
        if argn(2) == 3 then
            // d must be a string or a scalar -> check type and size
            d = varargin(2);
            if and(type(d)<> [1, 10]) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumsum",2))
            end

            if size(d,"*")<>1 then
                if type(d)==10 then
                    error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum",2))
                else
                    error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumsum",2))
                end
            end

            // call cumsum(a,d,typ), so d must be "m", "c", "r", "*" or an integer > 0
            if type(d)==10 then
                if and(d <> ["m","*","r","c"]) then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(varargin(1)))))
                end
            else
                if d<0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
                end
            end

            // typ must be a string and typ = "double" -> check type, size
            typ = varargin($)
            if type(typ)<>10 then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"cumsum",3))
            end

            if size(typ,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum",3))
            end

            if typ <> "double" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"cumsum", 3, "native", "double"));
            end
            varargin($) = null()
        end
        a=cumsum(varargin(:))
    end
endfunction
