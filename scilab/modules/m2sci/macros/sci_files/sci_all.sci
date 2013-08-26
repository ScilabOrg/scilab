// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_all(tree)
    // File generated from sci_PROTO13.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab all()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_all()

    tree.name="and"

    // B = all(A)
    if rhs==1 then
        A = getrhs(tree)

        // %c_and and %b_and are not defined
        A = convert2double(A)

        if is_complex(A) then
            A = Funcall("abs",1,Rhs_tlist(A),list(Variable("",A.infer)))
        elseif ~is_real(A) then
            newA = Funcall("abs",1,Rhs_tlist(A),list(Variable("",A.infer)))
            repl_poss(newA,A,A,gettext("is Real."))
            A=newA
        end
        tree.rhs=Rhs_tlist(A)

        // if A is not a multidimensional array
        if size(A.dims)==2 then
            if is_a_vector(A) then
                tree.lhs(1).dims=list(1,1)
            elseif not_a_vector(A) then
                tree.rhs=Rhs_tlist(A,1)
                tree.lhs(1).dims=list(1,A.dims(2))
            else
                tree.name="mtlb_all"
                tree.lhs(1).dims=list(Unknown,Unknown)
            end
            // if A is a multidimensional array
        else
            dim = first_non_singleton(A)
            if dim<>Unknown then
                tree.rhs=Rhs_tlist(A,dim)
                tree.lhs(1).dims=A.dims
                tree.lhs(1).dims(dim)=1
            else
                newrhs=Funcall("firstnonsingleton",1,list(A),list())
                tree.rhs=Rhs_tlist(A,newrhs)
                tree.lhs(1).dims=allunknown(A.dims)
            end
        end
        tree.lhs(1).type=Type(Boolean,Real)

        // B = all(A,dim)
    else
        [A,dim] = getrhs(tree)

        // %c_and and %b_and are not defined
        A = convert2double(A)

        if is_complex(A) then
            A = Funcall("abs",1,Rhs_tlist(A),list(Variable("",A.infer)))
        elseif ~is_real(A) then
            newA = Funcall("abs",1,Rhs_tlist(A),list(Variable("",A.infer)))
            repl_poss(newA,A,A,gettext("is Real."))
            A=newA
        end
        tree.rhs=Rhs_tlist(A,dim)

        if typeof(dim)=="cste" then
            if dim.value<=size(A.dims) then
                tree.lhs(1).dims=A.dims
                tree.lhs(1).dims(dim.value)=1
                tree.lhs(1).type=Type(Boolean,Real)
            else
                if not_empty(A) then
                    tree=Operation("<>",list(A,Cste(0)),tree.lhs)
                    tree.out(1).dims=A.dims
                    tree.out(1).type=Type(Boolean,Real)
                else
                    tree.name="mtlb_all"
                    tree.lhs(1).dims=A.dims
                end
            end
        else
            tree.name="mtlb_all"
            tree.lhs(1).dims=allunknown(A.dims)
        end
    end
endfunction
