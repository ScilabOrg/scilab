// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function s=%ce_e(varargin)
    //extraction from struct
    w=varargin($);
    if type(varargin(1))==15  //x(i,j,k).f or x.f(i,j,k)
        index=varargin($-1);
        if type(index($))==10 then  //x(i,j,k).f
            f=index($);
            //ijk=list2vec(index(1));
            [indx,I]=convertindex(double(w.dims),index(1)(:));
            // I<->ijk
            all_fields=getfield(1,w);all_fields=all_fields(3:$);
            fieldnum=find(f==all_fields);
            if fieldnum==[] then error(msprintf(_("%s: Invalid field name.\n"),"%ce_e"));end
            good_list=getfield(2+fieldnum,w);
            if size(I,"*")==1 then
                s=good_list(I)  //list(good_list(I)) ?
            else
                s=list();
                for kk=1:size(I,"*")
                s(kk)=good_list(I(kk));end
            end
            return;
        else //x.f(i,j,k)
            f=index(1);
            li=w(f);
            [indx,I]=convertindex(double(w.dims),index($)(:));
            s=li(I);
            return;
        end
    end

    if type(varargin(1))==10 then
        //field  x.f
        fields=getfield(1,w);nu=find(fields(3:$)==varargin(1));
        if nu==[] then
            error(msprintf(_("%s: Wrong value for input argument #%d: a valid field name expected.\n"),"%ce_e",1));return;
        end
        s=getfield(nu+2,w);
        if prod(getfield(2,w))==1 then s=s(1);end
        return;
    end

    //substruct x(i,j,k...)
    nind=size(varargin)-1

    dims1=double(w.dims)
    if nind<size(dims1,"*") then
        //reduce the dimension according to the number of indexes
        dims1=[dims1(1:nind-1) prod(dims1(nind:$))]
        if size(dims1,"*")==1 then dims1=[dims1 1],end
    end
    //get the "linear" vector of indexes
    [indx,I]=convertindex(dims1,varargin(1:$-1));
    dims=zeros(1,nind)

    //computing dimensions of the result
    //following loop should be merged in the convertindex code
    for kk=1:nind
        ik=varargin(kk)

        if or(type(ik)==[2 129]) then
            ik=horner(ik,dims1(kk))
        end
        if type(ik)==4 then
            dims(kk)=max(find(ik))
        elseif size(ik,1)~=-1 then
            dims(kk)=size(ik,"*")
        else
            dims(kk)=dims1(kk)
        end
    end

    while  dims($)==1 then dims($)=[],end
    select size(dims,"*")
    case 0 then
        dims=[1,1]
    case 1 then
        dims=[dims,1]
    else
        dims=matrix(dims,1,-1)
    end
    s=mlist(getfield(1,w),int32(dims));
    sz=size(getfield(1,w),"*");
    for k=3:sz
        ww=getfield(k,w);
        if type(ww)~=15 then ww=list(ww);end
        if prod(dims)==1
            setfield(k,ww(I),s);
        elseif prod(dims)==0 //VC 27/05/2004 after modification in convertindex
            setfield(k,list(),s);
        else
            setfield(k,list(ww(I)),s);
        end
    end
endfunction
