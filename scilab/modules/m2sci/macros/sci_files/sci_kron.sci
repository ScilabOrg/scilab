// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_kron(tree)
    // M2SCI function
    // Conversion function for Matlab kron()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // Scilab .*. operator does no work with Strings...
    if tree.rhs(1).vtype<>Double then
        tree.rhs(1)=convert2double(tree.rhs(1))
    end
    if tree.rhs(2).vtype<>Double then
        tree.rhs(2)=convert2double(tree.rhs(2))
    end

    tree=Operation(".*.",tree.rhs,tree.lhs)

    tree.out(1).type=Type(Double,Unknown)

    if is_a_scalar(tree.operands(1)) then
        tree.out(1).dims=tree.operands(2).dims
    elseif is_a_scalar(tree.operands(2)) then
        tree.out(1).dims=tree.operands(1).dims
    end
endfunction
