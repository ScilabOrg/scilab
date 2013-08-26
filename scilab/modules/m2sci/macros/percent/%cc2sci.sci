// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=%cc2sci(tree)

    // Make a 'column' with many rows
    if tree.operands(1).vtype==Cell then
        tree=%cc_cell2sci(tree)
        return
    end

    rownb=size(tree.operands)
    col=list()
    rowsize=[]
    colsize=[]

    realrows=0
    complexrows=0

    for k=1:rownb
        ck=tree.operands(k)
        if ck==list("EOL") | typeof(ck)=="comment" then
            rowsize=[rowsize 0]
            colsize=[colsize 0]
        else
            rowsize=[rowsize tree.operands(k).dims(1)]
            colsize=[colsize tree.operands(k).dims(2)]
            if ck.property==Complex then
                complexrows=complexrows+1
            end
            if ck.property==Real then
                realrows=realrows+1
            end
        end
    end

    if realrows==rownb then
        prop=Real
    elseif complexrows<>0 then
        prop=Complex
    else
        prop=Unknown
    end


    undef=find(colsize==-1)
    void=find(colsize==0)
    colsize([undef void])=[]
    if colsize==[] then
        if undef<>[] then
            sc=-1
        else
            sc=0
        end
    else
        [w,k]=min(length(colsize))
        sc=colsize(k)
    end
    if find(rowsize==-1)==[] then
        w=rowsize(1)
        for k=2:rownb
            w=w+rowsize(k)
        end
        sr=w
    else
        sr=-1
    end
    tree.out(1).dims=list(sr,sc)
    tree.out(1).type=Type(ck.vtype,prop)
    tree.out(1).contents=Contents()
endfunction
