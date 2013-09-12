// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function varargout=mtlb_sortrows(varargin)
    // Emulation function for sortrows() Matlab function

    [lhs,rhs]=argn()

    if rhs==1 then
        if type(varargin(1))==1 | type(varargin(1))==8 | type(varargin(1))==10 then
            [var1,index]=gsort(varargin(1),"lr","i")
        elseif type(varargin(1))==4 then
            [var1,index]=gsort(bool2s(varargin(1)),"lr","i")
        end
        varargout(1)=var1
    elseif rhs==2 then
        A=varargin(1)
        cls=varargin(2)
        indexlist=list((1:size(A,1))');

        if size(cls,2)>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Column vector expected.\n"),"mtlb_sortrows",2))
        end

        if type(A)==10 then
            B=asciimat(A)
        elseif type(A)==4
            B=bool2s(A)
        elseif type(A)==1
            B=A
        end

        for i=size(cls,1):-1:1
            if cls(i)>0
                [M,indextemp]=gsort(B(:,cls(i)),"lr","i")
            else
                [M,indextemp]=gsort(B(:,abs(cls(i))),"lr","d")
            end
            for j=1:size(indextemp,1)
                Btemp(j,:)=B(indextemp(j),:)
            end
            B=Btemp
            indexlist($+1)=indextemp
        end

        index=indexlist($)
        if size(indexlist)>2 then
            indextemp=zeros(size(A,1),1)
            for i=size(indexlist)-1:-1:1
                indexlist1=indexlist(i)
                indextemp=indexlist1(index)
                index=indextemp
            end
        end

        if type(A)==10 then
            varargout(1)=asciimat(B)
        else
            varargout(1)=B
        end
    end

    if lhs==2 then
        varargout($+1)=index
    end

endfunction