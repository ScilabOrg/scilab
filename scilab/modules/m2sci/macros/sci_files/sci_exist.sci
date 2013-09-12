// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_exist(tree)
    // M2SCI function
    // Conversion function for Matlab exist()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_exist()

    no_equiv(gettext("Matlab exist(), mtlb_exist() is called."))

    set_infos(gettext("mtlb_exist() is just a partial emulation of Matlab exist() function."),2)

    scitree.name="mtlb_exist"

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)

endfunction
