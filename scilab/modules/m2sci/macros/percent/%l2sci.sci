// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=%l2sci(tree)
    // M2SCI function
    // Conversion function for Matlab left division
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_l()

    [A,B] = getoperands(tree)

    // We have to call mtlb_l() for strings because result is computed with transposed strings...
    if or(A.vtype==[String,Unknown]) & or(A.vtype==[String,Unknown]) then
        tree=Funcall("mtlb_l",1,list(A,B),tree.out)
        tree.lhs(1).dims=allunknown(A.dims)
        if A.vtype==String & B.vtype==String then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    else
        // Boolean -> Double
        A = convert2double(A)
        B = convert2double(B)

        tree.operands=list(A,B)

        if is_real(A) & is_real(B) then
            tree.out(1).type=Type(Double,Real)
        elseif (is_real(A) & is_complex(B)) | (is_real(B) & is_complex(A)) then
            tree.out(1).type=Type(Double,Complex)
        else
            tree.out(1).type=Type(Double,Unknown)
        end

        if is_a_scalar(A) then
            tree.out(1).dims=B.dims
        elseif is_a_scalar(B) then
            tree.out(1).dims=A.dims
        else
            tree.out(1).dims=list(A.dims(2),B.dims(2))
        end
    end
endfunction

