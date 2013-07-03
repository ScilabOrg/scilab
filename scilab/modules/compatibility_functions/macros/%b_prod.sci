// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%b_prod(varargin)
    if varargin($)=="native" then
        if size(varargin)==3 & type(varargin(2))==1 & varargin(2)>2 then
            r=varargin(1)
        else
            // call prod(a, d) with d = "native"
            // or call(a, d, typ) with typ = "native"
            if size(varargin) == 3 then
                d = varargin(2);
                // d must be a string or scalar -> check type
                if and(type(d)<> [1, 10]) then
                    error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"prod",2))
                end

                if size(d,"*")<>1 then
                    if type(d)==10 then
                        error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",2))
                    else
                        error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
                    end
                end

                // If d is a string, d = "m", "c", "r" or "*"
                // Else d is an integer > 0
                if type(d)==10 then
                    d=find(d==["m","*","r","c"])
                    if d==[] then
                        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                        "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(varargin(1)))))
                    end
                    d=d-2
                else
                    if d<0 then
                        error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                        "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(varargin(1)))))
                    end
                end

                dims=size(varargin(1));

                if d==-1 then //'m'
                    d=find(dims>1,1)
                    if d==[] then d=0,end
                end
                
                // If d = 0, error in or function
                if d == 0 then
                    d = "*"
                end
                
                varargin(2) = d;
            end
            r=and(varargin(1:$-1))
        end
    else
        // converts boolean to double
        varargin(1)=bool2s(varargin(1))

        // call prod(a, d, typ)
        if argn(2) == 3 then
            // d must be a string or a scalar -> check type and size
            d = varargin(2);
            if and(type(d)<> [1, 10]) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"prod",2))
            end

            if size(d,"*")<>1 then
                if type(d)==10 then
                    error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",2))
                else
                    error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
                end
            end

            // If d is a string, d = "m", "*", "r", "c"
            // Else  d is an integer > 0
            if type(d)==10 then
                if and(d <> ["m","*","r","c"]) then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(varargin(1)))))
                end
            else
                if d < 0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(varargin(1)))))
                end
            end

            // typ must be a string and typ="double"
            typ = varargin($)
            if type(typ)<>10 then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"prod",3))
            end

            if size(typ,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",3))
            end

            if typ <> "double" then
                error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"prod", 3, "native", "double"));
            end
            varargin($) = null()
        end
        r = prod(varargin(:))
    end
endfunction
