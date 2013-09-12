// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%sp_q_s(a,b)
    //r=a.\b

    [ij,v,mn]=spget(a)
    if size(v,"*")<>mn(1)*mn(2) then
        error(27)
    else
        r=full(a).\b
    end
endfunction
