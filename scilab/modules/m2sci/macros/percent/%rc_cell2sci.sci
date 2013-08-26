// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tree=%rc_cell2sci(tree)

    // Make a CELL 'row' with many columns

    if typeof(tree.operands(1))=="funcall" then
        if tree.operands(1).name=="cell" then
            tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),Contents())
            tree.out(1).contents.index($+1)=list(list(Cste(1),Cste(1)),Cste("entries"))
            tree.out(1).contents.data($+1)=tree.operands(2).infer
            return
        end
    end

    if tree.operands(1).vtype==Cell then
        tree.out(1).infer=tree.operands(1).infer
    else
        tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),Contents())
        tree.out(1).contents.index($+1)=list(list(Cste(1),Cste(1)),Cste("entries"))
        tree.out(1).contents.data($+1)=tree.operands(1).infer
    end
    dim1=tree.operands(1).dims(1)
    dim2=tree.operands(1).dims(2)
    tree.out(1).contents.index($+1)=list(list(Cste(dim1),Cste(dim2+1)),Cste("entries"))
    tree.out(1).contents.data($+1)=tree.operands(2).infer

    if tree.out(1).dims(2)<>Unknown then
        tree.out(1).dims(2)=tree.out(1).dims(2)+1
    end
    if tree.out(1).dims(1)==0 then
        tree.out(1).dims(1)=tree.out(1).dims(1)+1
    end
endfunction
