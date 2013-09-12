// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tree=sci_acoth(tree)
    // M2SCI function
    // Conversion function for Matlab acoth()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    X=getrhs(tree)
    X=convert2double(X)

    set_infos(msprintf(gettext("If %s is outside [-1,1]\n   complex part of output of %s will be the opposite of Matlab one."),expression2code(X),expression2code(tree)),2)

    tree.name="atanh"
    Op=Operation("./",list(Cste(1),X),list())
    tree.rhs=Rhs_tlist(Op)

    tree.lhs(1).dims=X.dims
    // Property unknown because result can be complex for real input
    tree.lhs(1).type=Type(X.vtype,Unknown)
endfunction
