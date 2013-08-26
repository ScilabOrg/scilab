// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function   tree=sci_sortrows(tree)
    // M2SCI function
    // Conversion function for Matlab sortrows()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if size(tree.rhs)==1 then
        if tree.rhs(1).vtype==4
            tree.rhs(1)=convert2double(tree.rhs(1))
            tree=Funcall("gsort",1,list(tree.rhs(1),Cste("lr"),Cste("i")),list(tree.lhs(:)))
        elseif tree.rhs(1).vtype==Unknown
            tree=Funcall("mtlb_sortrows",1,list(tree.rhs(1)),list(tree.lhs(:)))
        else
            tree=Funcall("gsort",1,list(tree.rhs(1),Cste("lr"),Cste("i")),list(tree.lhs(:)))
        end
        tree.lhs(1).infer=tree.rhs(1).infer
    elseif size(tree.rhs)==2 then
        tree=Funcall("mtlb_sortrows",1,list(tree.rhs(1),tree.rhs(2)),list(tree.lhs(:)))
    end

    if size(tree.lhs)==2 then
        tree.lhs(2).dims=list(tree.rhs(1).dims(1),1)
        tree.lhs(2).type=Type(1,Real)
    end

endfunction
