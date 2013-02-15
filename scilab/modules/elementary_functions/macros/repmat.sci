// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2011 -  INRIA, Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function B = repmat(A,varargin)
    //ajouter test sur les type des elements de varargin
    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(_("%s: Wrong number of input arguments: at least %d expected.\n"), "repmat", 2))
    end
    flag=0; A_type=A;
    if type(A)==8 then A=iconvert(A,0); flag=1;
    end

    narg=size(varargin);
    
    // Test varargin
    if narg==1 then
        // Scalar of vector needed
        if typeof(varargin(1)) <> "constant" then
            error(msprintf(_("%s: Wrong type for input argument #%d: A real scalar or vector expected.\n"), "repmat", 2))
        end
        if size(varargin(1),'*')<>1 & isempty(find(size(varargin(1))==1)) then
            error(msprintf(_("%s: Wrong size for input argument #%d: A real scalar or vector expected.\n"), "repmat", 2))
        end
    else
        for i=1:narg
            if typeof(varargin(i)) <> "constant" then
                error(msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "repmat", i+1))
            end
            if size(varargin(i),"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "repmat", i+1))
            end
        end
    end

    if type(A)>10 then
        if typeof(A)=="rational" then
            B=rlist(repmat(A.num,varargin(:)),repmat(A.den,varargin(:)),A.dt)
            return
        elseif typeof(A)<>"hypermat" then
            execstr('B=%'+typeof(A)+"_repmat(A,varargin(:))")
            return
        end
    end

    if narg==1 then
        if size(varargin(1),'*')==1 then
            siz=list(varargin(1),varargin(1))
        else //convert array into list
            tmp=varargin(1)
            siz=list();
            for i=1:size(tmp,'*'),siz(i)=tmp(i); end
        end
    else
        siz=list();
        for i=1:narg
            siz(i)=varargin(i)
        end
    end

    nd=size(siz)
    if or(type(A)==[5 6]) then //sparse matrices
        if nd>2 then
            error(msprintf(_("%s: Wrong number of output matrix dimensions required: %d expected for sparse matrices.\n"), "repmat", 2))
        end
    end

    for i=size(siz):-1:3
        if siz(i)>1 then break,end
        nd=nd-1
    end
    sizA=size(A)
    nda=size(sizA,'*')
    if and(sizA==1) then //scalar case
        //this case can also be handled by the general one but in a less
        //efficient way
        if nd<=2 then
            B=A(ones(siz(1:nd)))
        else
            s=1;for k=1:nd;s=s*siz(k),end
            B=matrix(A(ones(s,1)),siz(1:nd))
        end
    else //general case
        if nda<nd then
            sizA(nda+1:nd)=1;
        elseif  nda>nd then
            for k=nd+1:nda
                siz(k)=1
            end
            nd=nda
        end

        I=list();
        for i=1:nd
            ind=matrix(1:sizA(i),-1,1);
            ind=ind(:,ones(1,siz(i)));
            I(i)=ind;
        end
        B=A(I(:))
    end

    if flag==1 then B=iconvert(B,inttype(A_type));
    end
endfunction
