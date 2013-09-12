// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_fft2(tree)
    // M2SCI function
    // Conversion function for Matlab fft2()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // Y = fft2(X)
    if rhs == 1 then
        X = getrhs(tree)
        // %c_fft2 and %b_fft2 are not defined
        X = convert2double(X)
        tree.rhs=Rhs_tlist(X)

        tree.lhs(1).dims=X.dims
        tree.lhs(1).type=Type(Double,Unknown)

        // Y = fft2(X,m,n)
    elseif rhs == 3 then
        [X,m,n] = getrhs(tree)
        X = convert2double(X)
        m = convert2double(m)
        n = convert2double(n)
        tree.rhs=Rhs_tlist(X,m,n)

        tree.lhs(1).type=Type(Double,Unknown)

        for i=1:size(X.dims)
            if X.dims(i)==-1 then
                tree.lhs(1).dims=allunknown(X.dims)
                return
            end
        end
        if tree.rhs(2).dims(1) <> -1 & tree.rhs(3).dims(1) <> -1
            tree.lhs(1).dims=list(m.value,n.value)
        else
            tree.lhs(1).dims=allunknown(X.dims)
            return
        end
        // case x hypermatrix
        for i=3:size(X.dims)
            tree.lhs(1).dims($+1)=X.dims(i)
        end
    end

endfunction
