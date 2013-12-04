// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x = %sp_inv(a)
    warning("off")
    [ma, na] = size(a)
    if ma<>na then 
        warning("on")
        error(20,1)
    end
    rk = det(a);
    if rk == 0 then
        warning("on")
        error(19);
    end
    Lup = umf_lufact(a);
    x = sparse(umf_lusolve(Lup, eye(ma, na)));
    umf_ludel(Lup)
    warning("on")

endfunction
