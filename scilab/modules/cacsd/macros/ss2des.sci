// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function S=ss2des(Sl,flag)
    // Returns S=list('des',A,B,C,D,E) for Sl a state-space
    // system with Ds polynomial.
    // if flag=="withD" a maximal rank D matrix is returned in S
    // otherwise D=0;
    // Copyright INRIA
    [LHS,RHS]=argn(0);
    if RHS==1 then flag=[];end
    if RHS==2&flag<>"withD" then warning(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"ss2des",2,"withD"));end
    Ds=Sl(5);
    if type(Ds)==1 then
        if norm(Ds,1)==0 then S=Sl;return;end
        if norm(Ds,1)<>0 then Sl(5)=Ds+poly(0,"s")*0*Ds;end
    end
    [A2,B2,C2,Ds]=Sl(2:5);
    if flag=="withD" then D=coeff(Ds,0);Ds=Ds-D;end
    [N,B1,C1]=pol2des(Ds);
    [n1,n1]=size(N);
    [n2,n2]=size(A2);
    E=[N,0*ones(n1,n2);0*ones(n2,n1),eye(n2,n2)];
    A=[eye(N),0*ones(n1,n2);0*ones(n2,n1),A2];
    C=[C1,C2];
    B=[B1;B2];
    if flag<>"withD" then D=0*C*B;end
    S=tlist(["des","A","B","C","D","E"],A,B,C,D,E);
endfunction
