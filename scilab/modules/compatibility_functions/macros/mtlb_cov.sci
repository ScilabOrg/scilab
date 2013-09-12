// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function Y=mtlb_cov(A,B,C)
    // Emulation function for Matlab cov()

    [lhs,rhs]=argn()
    if rhs==1
        if size(A,1)==1 & size(A,2)==1
            Y = mvvacov(A)
        elseif size(A,1)==1 & size(A,2)>1
            N = size(A,2)
            Y = (N/(N-1))*mvvacov(A')
        elseif size(A,1)>1
            N = size(A,1)
            Y = (N/(N-1))*mvvacov(A)
        end
    elseif rhs==2
        if  prod(size(B,"*"))==1 & and(B==1)
            if size(A,1)==1 & size(A,2)==1
                Y = mvvacov(A)
            elseif size(A,1)==1 & size(A,2)>1
                Y = mvvacov(A')
            elseif size(A,1)>1
                Y = mvvacov(A)
            end
        elseif prod(size(B,"*"))==1 & and(B==0)
            if size(A,1)==1 & size(A,2)==1
                Y = mvvacov(A)
            elseif size(A,1)==1 & size(A,2)>1
                N = size(A,2)
                Y = (N/(N-1))*mvvacov(A')
            elseif size(A,1)>1
                N = size(A,1)
                Y = (N/(N-1))*mvvacov(A)
            end
        elseif prod(size(B,"*"))==1
            B = matrix(B,-1,1)
            A = matrix(A,-1,1)
            Y = mvvacov([A,B])
        elseif prod(size(B,"*"))>1
            N = prod(size(B,"*"))
            B = matrix(B,-1,1)
            A = matrix(A,-1,1)
            Y = (N/N-1)*mvvacov([A,B])
        end
    elseif rhs==3
        B = matrix(B,-1,1)
        A = matrix(A,-1,1)
        if  C==1
            Y = mvvacov([A,B])
        elseif C==0
            N = prod(size(B,"*"))
            if N<>1
                Y = (N/(N-1))*mvvacov([A,B])
            else
                Y = mvvacov([A,B])
            end
        end
    end
endfunction