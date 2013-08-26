// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [A,jb]=rref(A,tol)
    //R = rref(A) produces the reduced row echelon form of A.
    if argn(2)<2 then
        tol=2*%eps*norm(A,"inf")*max(size(A))
    end

    if type(A)==1 then
        if A==[] then R=[],return,end
        [m,n]=size(A)
        k = 1;l = 1;jb = [];
        while (k <= m) & (l <= n)
            // Find value and index of largest element in the remainder of column l.
            [p,i] = max(abs(A(k:$,l))); i = i+k-1;
            if (p <= tol) // The column is negligible
                A(k:$,l) = zeros(m-k+1,1);
                l = l + 1;
            else
                jb = [jb, l]; // Remember column index
                A([k,i],l:$) = A([i,k],l:$); //swap rows i and j
                A(k,l:$) = A(k,l:$)/A(k,l); // Normalize the pivot row

                i = [1:k-1 k+1:m]; //other rows
                A(i,l:$) = A(i,l:$) - A(i,l)*A(k,l:$); //eliminate
                k = k + 1;l = l + 1;
            end
        end
    else
        [t,n]=typename();n=stripblanks(n(find(t==type(A))))
        fun="%"+n+"_rref"
        if exists(fun)==1 then
            execstr("[A,jb]="+fun+"(A)")
        else
            error(msprintf(gettext("%s: Function not defined for type ''%s''. Check argument or define function %s."),"rref",n,fun));
        end
    end
endfunction
