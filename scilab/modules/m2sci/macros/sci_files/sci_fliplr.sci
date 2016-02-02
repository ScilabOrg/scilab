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

function [tree]=sci_fliplr(tree)
    // M2SCI function
    // Conversion function for Matlab fliplr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_fliplr()

    treedims=tree.rhs(1).dims
    treetype=tree.rhs(1).type


    if or(tree.rhs(1).vtype==[String,Unknown]) then
        tree.name="mtlb_fliplr"
        tree.lhs(1).dims=treedims
        tree.lhs(1).type=treetype
    else
        if typeof(tree.rhs(1))<>"variable" then
            tmp=gettempvar()
            m2sci_insert(Equal(list(tmp),tree.rhs(1)))
        else
            tmp=tree.rhs(1)
        end

        ind1=Cste(":")
        ind2=Operation(":",list(Cste("$"),Cste(-1),Cste(1)),list())

        tree=Operation("ext",list(tmp,ind1,ind2),tree.lhs)

        tree.out(1).dims=treedims
        tree.out(1).type=treetype
    end
endfunction
