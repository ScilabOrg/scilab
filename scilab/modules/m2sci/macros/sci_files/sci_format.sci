// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_format(tree)
    // M2SCI function
    // Conversion function for Matlab format
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_format()

    rhs1=rhs
    if rhs==2 then
        fp="e"
        rhs=rhs-1
    else
        fp="v"
    end

    d=5;
    hex=%f;
    bank=%f;
    compact=%f;
    loose=%f,
    ratf=%f;
    symb=%f;
    unk=%f

    if rhs==1 then
        f=tree.rhs(1)
        select f.value
        case """short""" then
            d=5
        case """long""" then
            d=15
        case """hex""" then
            hex=%t
        case """+""" then
            symb=%t
        case """bank""" then
            bank=%t
        case """compact""" then
            compact=%t
        case """loose""" then
            loose=%t
        case """rat""" then
            ratf=%t
        else
            unk=%t
        end
    else
        d=5
    end


    if unk then
        if rhs1==1 then
            tree.name="mtlb_format"
        else
            tree.name="mtlb_format"
        end
    elseif ratf|loose|compact|bank|hex then
        no_equiv("format "+f.value)
    else
        if rhs1==1 then
            tree.rhs=Rhs_tlist(d+1)
        elseif rhs1==2 then
            tree.rhs=Rhs_tlist(fp,d+1)
        else
            tree.rhs=Rhs_tlist("v",6)
        end
    end
endfunction
