
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [U,S,V]=sva(A,tol)
    // approximation de rang donne d'une matrice
    if A==[] then U=[],S=[],V=[],return,end
    [U,S,V]=svd(A,"e")
    if argn(2)==1 then
        tol = max(size(A)) * S(1) * %eps;
        rk = size(find(diag(S) > tol),"*");
    else
        if tol>1 then //rank given
            rk=tol
            if rk>min(size(A)) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Requested rank is greater than matrix dimension."),"sva",1));
            end
        else
            rk = size(find(diag(S) > tol),"*");
        end
    end
    U=U(:,1:rk);S=S(1:rk,1:rk),V=V(:,1:rk)
endfunction



