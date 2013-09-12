// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=%h2sci(tree)
    // M2SCI function
    // Conversion function for Matlab logical AND
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    // Overloading functions in $SCI/modules/compatibility_functions/macros/:
    // - %b_h_s.sci
    // - %s_h_b.sci
    // These functions are not used to get the same output value as Matlab one with empty matrices

    // %s_h_s is also defined but no more used (hard coded)

    [A,B]=getoperands(tree)

    // Short circuiting AND
    if (typeof(B)=="variable" & B.name=="%shortcircuit") then
        if typeof(tree.out(1))=="variable" & tree.out(1).name=="ans" then
            tmp=gettempvar()
            tmp.type=Type(Boolean,Real)
            tree=tmp
        else
            tmp=tree.out(1)
            global("varslist")
            varslist($+1)=M2scivar(tree.out(1).name,tree.out(1).name,Infer(list(1,1),Type(Boolean,Real)))
            tree=list()
        end
        insert(Equal(list(tmp),Cste(%F)))
        insert(tlist(["ifthenelse","expression","then","elseifs","else"],A.operands(1),list(Equal(list(tmp),A.operands(2))),list(),list()))
        return
    end

    // To have good size for result with String as input
    // And overloading functions are not written for Strings
    if A.vtype==Unknown | A.vtype==String then
        A = convert2double(A)
    end
    if B.vtype==Unknown | B.vtype==String then
        B = convert2double(B)
    end
    tree.operands=list(A,B)

    tree.out(1).type=Type(Boolean,Real)
    // If A is a scalar
    if is_a_scalar(A) then
        tree.out(1).dims=B.dims
        // If B is a scalar
    elseif is_a_scalar(B) then
        tree.out(1).dims=A.dims
        // If A or B is an empty matrix
    elseif is_empty(A) | is_empty(B) then
        tree.out(1).dims=A.dims
        // A and B are not scalars and not empty matrices -> they have the same size
    elseif not_empty(A) & not_empty(B) then
        tree.out(1).dims=A.dims
    else
        tree.out(1).dims=allunknown(A.dims)
    end

endfunction
