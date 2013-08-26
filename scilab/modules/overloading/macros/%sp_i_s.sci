// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M=%sp_i_s(varargin)
    // %sp_i_s(i,j,b,a) insert sparse matrix b into full matrix a
    // M(i,j)=b
    //!

    [lhs,rhs]=argn(0)

    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix
    index=varargin(1) //

    if rhs==3&(type(index)==10|type(index)==15) then
        M=createstruct(index,N)
        if type(index(1))<>10 & index(2)=="entries" then
            // change struct to cell
            f=getfield(1,M);f(1)="ce"
            setfield(1,f,M)
        end
    elseif rhs>4 then //more than 2 indices:

        //insertion of a sparse matrix in a matrix of numbers
        error(msprintf(_("%s: Insertion of a sparse matrix in a matrix of numbers is not defined.\n"),"sp_i_s"));
    else
        i=varargin(1);
        j=varargin(2);
        M=varargin(4);
        b=varargin(3);
        if rhs==4 then
            M(i,j)=full(b)
        else
            M=b;//b=j
            M(i)=full(j)
        end
    end
endfunction
