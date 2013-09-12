// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_strfind(tree)
    // M2SCI function
    // Conversion function for Matlab strfind()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_strfind()

    [A,B] = getrhs(tree)

    if A.vtype==String & B.vtype==String then
        tree.name="strindex"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Double,Real)
    else
        tree.name="mtlb_strfind"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Double,Real)
    end
endfunction
