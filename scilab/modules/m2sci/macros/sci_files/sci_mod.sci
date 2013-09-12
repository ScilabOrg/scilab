// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_mod(tree)
    // M2SCI function
    // Conversion function for Matlab mod()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_r_c and %b_r_b are not defined in Scilab
    [X,Y] = getrhs(tree)
    X = convert2double(X)
    Y = convert2double(Y)
    tree.rhs=Rhs_tlist(X,Y)

    tree.name="pmodulo"

    if is_a_scalar(X) then
        tree.lhs(1).dims=Y.dims
    elseif is_a_scalar(Y) then
        tree.lhs(1).dims=X.dims
    elseif X.dims==Y.dims then
        tree.lhs(1).dims=X.dims
    else
        tree.lhs(1).dims=allunknown(X.dims)
    end
    tree.lhs(1).type=Type(Double,Real)
endfunction
