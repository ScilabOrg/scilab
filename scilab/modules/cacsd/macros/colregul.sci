// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [Stmp,Ws]=colregul(Sl,Alfa,Beta);
    // [Stmp,Ws]=regul(Sl) computes a polynomial-state-space prefilter
    // Ws such that Stmp=Sl*Ws is proper and non singular.
    // Poles at infinity of Sl are moved to Alfa;
    // Zeros at infinity of Sl are moved to Beta;
    // Sl is asummed left invertible i.e. ss2tf(Sl) full column rank
    //!

    [LHS,RHS]=argn(0);
    if RHS==1 then Alfa=0;Beta=0;end
    flag=0;
    Sl1=Sl(1)
    if Sl1(1)=="r" then
        flag=1;Sl=tf2ss(Sl);
    end
    s=poly(0,"s");
    D=Sl(5);
    n=size(D);n1=n(1);n2=n(2);
    Ws=syslin([],[],[],[],eye(n2,n2));
    Stmp=Sl;
    m=max(degree(D));
    //     moving poles @ infinity to poles @ Alfa
    while m>0
        Dm=coeff(D,m);
        [W,r]=colcomp(Dm);
        if r==0 then Wstmp=W; else
            dim=n2-r;Wstmp=W*[eye(dim,dim),zeros(dim,r);
        zeros(r,dim),tf2ss(1/(s-Alfa)*eye(r,r))];end
        Ws=Ws*Wstmp;
        Stmp=Stmp*Wstmp;
        D=clean(Stmp(5));
        m=max(degree(D));
    end
    Stmp(5)=coeff(Stmp(5),0);

    [W,r]=colcomp(Stmp(5));
    if r==n1 then
        Ws=Ws*W;Stmp=Stmp*W;
        if flag==1 then Ws=ss2tf(Ws);Stmp=ss2tf(Stmp);end
        return;
    end
    [nx,nx]=size(Stmp(2));
    //      moving zeros @ infinity to zeros @ Beta
    i=0;
    while r < n2,
        i=i+1;
        if r==n2 then Wstmp=W;
        else
            dim=n2-r;Wstmp=W*[(s-Beta)*eye(dim,dim),zeros(dim,r);
            zeros(r,dim),eye(r,r)];
        end
        Wstmp=syslin([],[],[],[],Wstmp);
        Ws=Ws*Wstmp;
        Stmp=Stmp*Wstmp;
        Stmp(5)=coeff(Stmp(5),0);
        rold=r;
        [W,r]=colcomp(Stmp(5));
        //  if r==rold&r<>0 then break;end
        if i>=nx then break;end
    end
    if flag==1 then
    Ws=ss2tf(Ws);Stmp=ss2tf(Stmp);end
endfunction
