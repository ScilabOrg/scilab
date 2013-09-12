// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function des=sm2des(sysmat,n)
    // Generalized system-matrix to descriptor
    // sysmat=[-s E + A , B; C , D]
    // des=list('des',A,B,C,D,E)
    // n=size(A) (assumed square) (optional parameter)

    [LHS,RHS]=argn(0)
    [nl,nc]=size(sysmat);
    E=-coeff(sysmat,1);
    if RHS==1 then
        k=nc;n=k;
        while E(:,k)==0*E(:,k)
            k=k-1;n=k;
        end
        l=nl;n1=l;
        while E(l,:)==0*E(l,:)
            l=l-1;n1=l;
        end
        if n1<>n then
            warning(msprintf(gettext("%s: Cannot find the state dimension.\n"),"sm2des"));
        end
    end
    E=E(1:n,1:n);
    A=coeff(sysmat(1:n,1:n),0);
    B=coeff(sysmat(1:n,n+1:nc),0);
    C=coeff(sysmat(n+1:nl,1:n),0);
    D=coeff(sysmat(n+1:nl,n+1:nc),0);
    des=tlist(["des","A","B","C","D","E"],A,B,C,D,E);
endfunction
