// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%spb_cumprod(a,d,typ)
    rhs=argn(2)
    if rhs==1 then
        typ=list()
        d=0 //"*"
    else
        // call cumprod(a, d) or cumprod(a, d, typ)
        // d must be a string or scalar -> check type or size
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
        if rhs == 2 & or(d==["native","double"]) then
            typ=list(d)
            d=0 //"*"
        else
            // If d is a string, d = "m", "r", "c", "*"
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

            // call cumprod(a, d, typ)
            // typ must be a string, equal to "native" or "double"
            if rhs == 3  then
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
                typ=list()
            end
        end
    end

    dims=size(a);

    if d==-1 then //'m'
        d=find(dims>1,1)
        if d==[] then d=0,end
    end

    r=sparse(dims,0,dims)
    select d
    case 0 then
        [ij,v]=spget(a)
        if dims(1)==1 then
            l=find(diff(ij(:,2))>1,1)
            if l==[] then
                r=sparse(ij,cumprod(v,typ(:)),dims)
            else
                r=sparse(ij(1:l,:),cumprod(v(1:l),typ(:)),dims)
            end
        elseif dims(2)==1 then
            l=find(diff(ij(:,1))>1,1)
            if l==[] then
                r=sparse(ij,cumprod(v,typ(:)),dims)
            else
                r=sparse(ij(1:l,:),cumprod(v(1:l),typ(:)),dims)
            end
        else
            r=matrix(cumprod(matrix(a,1,-1),typ(:)),dims)
        end
    case 1 then
        ij=[];v=[];
        for k=1:dims(2)
            [ijk,vk]=spget(a(:,k));
            if and(ijk(1,:)==[1 1]) then
                l=find(diff(ijk(:,1))>1,1);
                if l==[] then
                    ij=[ij;[ijk(:,1) k*ones(vk)]];
                    v=[v;cumprod(vk,typ(:))];
                else
                    ij=[ij;[ijk(1:l,1) k*ones(l,1)]];
                    v=[v;cumprod(vk(1:l,:),typ(:))];
                end
            end
        end
        r=sparse(ij,v,dims)
    case 2 then
        ij=[];v=[]
        for k=1:dims(1)
            [ijk,vk]=spget(a(k,:))
            if and(ijk(1,:)==[1 1]) then
                l=find(diff(ijk(:,2))>1,1)
                if l==[] then
                    ij=[ij;[k*ones(vk) ijk(:,2)]];
                    v=[v;cumprod(vk,typ(:))]
                else
                    ij=[ij;[k*ones(l,1),ijk(1:l,2)]];
                    v=[v;cumprod(vk(1:l,:),typ(:))]
                end
            end
        end
        r=sparse(ij,v,dims)
    else
        if typ==list("native")
            r=a
        else
            r=bool2s(a)
        end
    end

endfunction
