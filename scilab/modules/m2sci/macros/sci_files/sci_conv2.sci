// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_conv2(tree)
    // M2SCI function
    // Conversion function for Matlab conv2()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==2 then // conv2(A,B)
        [A,B]=getrhs(tree)
        A=convert2double(A)
        B=convert2double(B)
        if is_real(A)&is_real(B) then
            tree.lhs(1).type=Type(Double,Real)
        elseif is_complex(A)|is_complex(B) then
            tree.lhs(1).type=Type(Double,Complex)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    elseif rhs==3 then
        [A,B,C]=getrhs(tree)
        if C.type==String then // conv(A,B,frame)
            A=convert2double(A)
            B=convert2double(B)
            if is_real(A)&is_real(B) then
                tree.lhs(1).type=Type(Double,Real)
            elseif  is_complex(A)|is_complex(B) then
                tree.lhs(1).type=Type(Double,Complex)
            else
                tree.lhs(1).type=Type(Double,Unknown)
            end
        else // conv(u,v,B)
            A=convert2double(A)
            B=convert2double(B)
            C=convert2double(C)
            if is_real(A)&is_real(B)&is_real(C) then
                tree.lhs(1).type=Type(Double,Real)
            elseif is_complex(A)|is_complex(B)|is_complex(C) then
                tree.lhs(1).type=Type(Double,Complex)
            else
                tree.lhs(1).type=Type(Double,Unknown)
            end
        end
    elseif rhs==4 then  // conv(u,v,B, frame)
        [A,B,C,D]=getrhs(tree)
        A=convert2double(A)
        B=convert2double(B)
        C=convert2double(C)
        if is_real(A)&is_real(B)&is_real(C) then
            tree.lhs(1).type=Type(Double,Real)
        elseif is_complex(A)|is_complex(B)|is_complex(C) then
            tree.lhs(1).type=Type(Double,Complex)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    end
    tree.lhs(1).dims=list(Unknown,Unknown);
endfunction
