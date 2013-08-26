// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=%i2sci_r(tree)
    // M2SCI function
    // Conversion function for Matlab insertion in row vectors (called by %i2sci())
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    from=tree.operands($)
    to=tree.operands(1)
    ind=tree.operands(2)

    if from.dims(1)==1 & from.dims(2)==1 then // Insert a scalar
    elseif from.dims(1)==1 then // Insert a row vector
    elseif from.dims(1)<>Unknown & from.dims(2)<>Unknown then // Insert a matrix with known sizes
        tree.operands($)=Funcall("matrix",1,Rhs_tlist(from,1,Operation("-",list(Cste(1)),list())))
    else
        if ~isdefinedvar(from) then
            w=gettempvar()
            insert(Equal(list(w),from))
        else
            w=from
        end

        // from=from(:).'
        tmp=Operation("ext",list(w,Cste(":")),list())
        tmp=Operation(".''",list(tmp),list())

        tree.operands(4)=tmp
        tree.operands(3)=ind
        tree.operands(2)=Cste(1)
    end

    // Data inference
    tree.out(1).dims=list(1,Unknown)
    tree.out(1).type=to.type
endfunction


