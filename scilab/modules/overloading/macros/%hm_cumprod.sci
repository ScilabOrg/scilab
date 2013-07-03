// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=%hm_cumprod(m,d,typ)
    rhs = argn(2);
    dims=m.dims;
    if rhs==1 then
        typ=list()
        d=0 //"*"
    else
        // call cumprod(m, d) or cumprod(m, d, typ)
        // d must be a string or scalar -> check type and size
        if and(type(d) <> [1, 10]) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A string or scalar expected.\n"),"cumprod",2))
        end

        if size(d,"*")<>1 then
            if type(d)==10 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",2))
            else
                error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod",2))
            end
        end

        // call cumprod(m, d) with d = "native" or "double"
        if rhs == 2 & or(d==["native","double"]) then
            typ=list(d)
            d=0  //"*"
        else
            // If d is a string, d = "m", "r", "c" or "*"
            // Else d is an integer > 0
            if type(d)==10 then
                d=find(d==["m","*","r","c"])
                if d==[] then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(m))))
                end
                d=d-2
            else
                if d<0 then
                    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                    "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(m))))
                end
            end

            if d==-1 then //'m'
                d=find(dims>1,1)
                if d==[] then d=0,end
            end

            // call cumprod(m, d, typ)
            // if m is an hypermatrix of integers or booleans,
            // typ must be checked
            // typ must be a string, equals to "native" or "double"
            if rhs == 3 then
                if or(type(m.entries) == [4 8]) then

                    if type(typ)<>10 then
                        error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"cumprod",3))
                    end

                    if size(typ,"*")<>1 then
                        error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",3))
                    end

                    if and(typ <> ["native", "double"])  then
                        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"cumprod", 3, "native", "double"));
                    end

                    typ=list(typ)
                else
                    // typ is omitted
                    warning(msprintf(_("%s: The argument #%d is only used for matrices of integers or booleans.\n"), "cumprod", 3));
                    typ=list();
                end
            else
                typ = list();
            end
        end
    end

    if d==0 then// '*'
        x=hypermat(m.dims,cumprod(m.entries,typ(:)))
        return
    end
    if d>size(dims,"*") then
        //requested summation direction exceeds array dims, return the array, converted
        //to double if necessary.
        tm=type(m.entries)
        if (tm==8&typ==list("double"))|(tm==4&typ<>list("native")) then
            m.entries=double(m.entries),
        end
        x=m
        return
    end

    if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
    N=size(dims,"*");
    p1=prod(dims(1:d-1));//summation step
    p2=p1*dims(d);//step for next to sum
    ind=(0:p1:p2-1);// selection for summation
    deb=(1:p1)';
    I=deb*ones(ind)+ones(deb)*ind


    ind=(0:p2:prod(dims)-1)';
    I=ones(ind).*.I+ind.*.ones(I)
    x=cumprod(matrix(m.entries(I),-1,dims(d)),2,typ(:))
    x(I)=matrix(x,-1,1)
    if flag==1 then dims=int32(dims);end
    x=hypermat(dims,x)
endfunction
