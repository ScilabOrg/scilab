// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - A. Khorshidi
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [D]=block_diag(varargin)
    // Syntax:
    // 		block_diag(A,B,C,...)
    // Parameters:
    //      A,B,C,...: constant, polynomail or rational matrices of any size
    // Summary:
    //      Build a block diagonal (sparse) matrix from provided matrices.
    // Example:
    //      A=[1 0; 0 1], B=[3 4 5; 6 7 8], C=7, D=block_diag(A,B,C)
    // Ref:
    //      http://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.block_diag.html

    rhs=argn(2); // rhs=length(varargin)
    for i=1:rhs
        [nrow(i),ncol(i)]=size(varargin(i))
    end

    D=zeros(sum(nrow),sum(ncol));

    m=cumsum([1;nrow]);
    n=cumsum([1;ncol]);
    for i=1:rhs
        D(m(i):m(i+1)-1, n(i):n(i+1)-1)=varargin(i);
    end

endfunction
