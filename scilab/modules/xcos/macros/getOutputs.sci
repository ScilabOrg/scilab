// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function Y=getOutputs(outtb,pointo)
    Yind=1
    for k=pointo
        nk=size(outtb(k),"*")
        Y(Yind:Yind+nk-1) = outtb(k)(:);
        Yind = Yind+nk
    end
endfunction
