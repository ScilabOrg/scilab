// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_pow2(tree)
    // M2SCI function
    // Conversion function for Matlab pow2()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // X = pow2(Y)
    if rhs==1 then
        // %c_pow2 and %b_pow2 are not defined in Scilab
        Y = getrhs(tree)
        Y = convert2double(Y)

        tree=Operation(".^",list(Cste(2),Y),list(Variable(tree.lhs(1).name,Y.infer)))
        // X = pow2(E,F)
    else
        // %c_pow2 and %b_pow2 are not defined in Scilab
        [E,F] = getrhs(tree)
        E = convert2double(E)
        F = convert2double(F)

        // Imaginary parts are ignored by Matlab
        if is_complex(E) then
            E = Funcall("real",1,list(E),list(Variable("",E.infer)))
        elseif ~is_real(E) then
            newE = Funcall("real",1,list(E),list(Variable("",E.infer)))
            repl_poss(newE,E,E,gettext("is Real."));
            E=newE
        end
        if is_complex(F) then
            F = Funcall("real",1,list(F),list(Variable("",F.infer)))
        elseif ~is_real(F) then
            newF = Funcall("real",1,list(F),list(Variable("",F.infer)))
            repl_poss(newF,F,F,gettext("is Real."));
            F=newF
        end

        tmp=Operation(".^",list(Cste(2),F),list(Variable("",F.infer)))

        tree=Operation(".*",list(E,tmp),list(Variable(tree.lhs(1).name,Infer())))

        if is_a_scalar(E) then
            tree.out(1).dims=F.dims
        elseif is_a_scalar(F) then
            tree.out(1).dims=E.dims
        elseif E.dims==F.dims then
            tree.out(1).dims=E.dims
        else
            tree.out(1).dims=allunknown(E.dims)
        end
        tree.out(1).type=Type(Double,Real)
    end


endfunction
