// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [res1, res2]=%sp_det(A)
    [lhs, rhs]=argn(0);
    [hand, rk]=lufact(A);
    [P,L,U,Q]=luget(hand);
    res1=prod(diag(L));
    res2=res1;
    if (lhs == 2) then
        res1=0;
        while abs(res2) >= 10
            if abs(res2) < 1 then
                break;
            end
            res2 = res2 / 10;
            res1 = res1 + 1;
        end
    end
    ludel(hand);
endfunction
