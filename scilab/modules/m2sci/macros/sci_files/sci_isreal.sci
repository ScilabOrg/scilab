// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_isreal(tree)
    // M2SCI function
    // Conversion function for Matlab isreal()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_isreal and %b_isreal are not defined in Scilab
    A = getrhs(tree)
    A = convert2double(A)
    tree.rhs=Rhs_tlist(A)

    // eps set to 0
    tree.rhs=Rhs_tlist(A,0)

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Boolean,Real)
endfunction
