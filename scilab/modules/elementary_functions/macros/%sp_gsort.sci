// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function A = %sp_gsort(A, optsort, directionsort)
    if ~exists("optsort") then optsort = 'g'; end
    if ~exists("directionsort") then directionsort = 'd'; end

    [ij, v, mn] = spget(A);
    if mn(2) == 1 | mn(1) == 1 then
        v = gsort(v,'g',directionsort);
        A = sparse(ij, v, mn);
    else
        error(999,msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"),'gsort',1));
    end
endfunction
