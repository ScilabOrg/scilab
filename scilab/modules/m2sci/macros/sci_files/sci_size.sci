// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_size(tree)
    // M2SCI function
    // Conversion function for Matlab size()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_size()

    // In Matlab : size('str1') = [1 4] but in Scilab : size('str1') = [1 1]
    // so we convert expr to ascii code matrix which have good size


    // m = size(X,dim) (If two inputs, then only one output)
    if rhs==2 then
        [X,dim]=getrhs(tree)
        if or(X.vtype==[String,Unknown]) then
            X = convert2double(X)
        end
        tree.rhs=Rhs_tlist(X,dim)

        // Matlab can work with dim > size(size(X),2) but not Scilab
        if typeof(dim)=="cste" then
            if dim.value>size(X.dims) then
                set_infos(msprintf(gettext("M2SCI found: %s > size(size(%s),2),\nSo result is set to 1."),expression2code(dim),expression2code(X)),0)
                tree=Cste(1)
            else
                tree.lhs(1).dims=list(1,1)
                tree.lhs(1).type=Type(Double,Real)
            end
        else
            tree=Funcall("mtlb_size",1,list(X,dim),tree.lhs)
            tree.lhs(1).dims=list(1,1)
            tree.lhs(1).type=Type(Double,Real)
        end

        // [d1,d2,d3,...,dn] = size(X)
    else
        X = getrhs(tree)
        if or(X.vtype==[String,Unknown]) then
            X = convert2double(X)
        end
        tree.rhs=Rhs_tlist(X)

        // d1 = size(X)
        if lhs==1 then
            tree.lhs(1).dims=list(1,size(X.dims))
            tree.lhs(1).type=Type(Double,Real)

            // [d1,d2,d3,...,dn] = size(X)
        else
            if size(tree.lhs)<size(X.dims) then
                // Number of outputs is less than size(size(X),2) so mtlb_size() is called
                tree=Funcall("mtlb_size",size(tree.lhs),list(X),tree.lhs)
            end
            if size(tree.lhs)>size(X.dims) then
                // Number of outputs is greater than size(size(X),2) so mtlb_size() is called
                tree=Funcall("mtlb_size",size(tree.lhs),list(X),tree.lhs)
            end
            for klhs=1:size(tree.lhs)
                tree.lhs(klhs).dims=list(1,1)
                tree.lhs(klhs).type=Type(Double,Real)
            end
        end
    end
endfunction
