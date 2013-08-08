// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function outtb=setInputs(outtb,pointi,U)
    Uind=1
    for k=pointi
        nk=size(outtb(k),"*")
        outtb(k) = matrix(U(Uind:Uind+nk-1),size(outtb(k)));
        Uind = Uind+nk;
    end
endfunction
