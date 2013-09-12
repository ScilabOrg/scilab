// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function M=%hm_i_b(varargin)
    //insertion of an hypermatrix in a matrix  derived from %hm_i_hm
    [lhs,rhs]=argn(0)
    M=varargin(rhs) //Matrix
    N=varargin(rhs-1)//inserted hypermatrix
    dims=size(M)'

    v=M(:)


    nd=size(dims,"*")
    if rhs-2>nd then dims(nd+1:rhs-2)=1;end

    //convert N-dimensional indexes to 1-D
    [Ndims,I]=convertindex(list(dims,double(matrix(N.dims,1,-1))),varargin(1:$-2))

    if or(Ndims>dims) then
        //extend the destination matrix
        I1=0
        for k=size(Ndims,"*"):-1:1
            ik1=(1:dims(k))'
            if ik1<>[] then
                if Ndims(k)>1 then
                    if size(I1,"*")>1 then
                        I1=(Ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
                    else
                        I1=Ndims(k)*I1+ik1-1
                    end
                else
                    I1=Ndims(k)*I1+ik1-1
                end
            end
        end
        select type(v)
        case 1
            v1=zeros(prod(Ndims),1)
        case 2 then
            v1=zeros(prod(Ndims),1)
        case 4 then
            v1=(zeros(prod(Ndims),1)==1)
        case 8 then
            v1=iconvert(zeros(prod(Ndims),1),inttype(v))
        case 10 then
            v1=emptystr(prod(Ndims),1)
        end
        v1(I1+1)=v;v=v1
    end

    v(I)=matrix(N.entries,-1,1)

    while  Ndims($)==1 then Ndims($)=[],end
    select size(Ndims,"*")
    case 0
        M=v
    case 1
        M=v
    case 2
        M=matrix(v,Ndims(1),Ndims(2))
    else
        M=mlist(["hm","dims","entries"],matrix(Ndims,1,-1),v)
    end
endfunction
