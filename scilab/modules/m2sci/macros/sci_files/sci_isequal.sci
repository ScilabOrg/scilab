// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_isequal(tree)
    // M2SCI function
    // Conversion function for Matlab isequal()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if tree.rhs(1).vtype==Struct then
        set_infos(gettext("isequal requires fields are in the same order for structs to be equal."),2);
    end

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Boolean,Real)
endfunction
