// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function [S,Sn]=arma2ss(Ar)
    //Ar : an armax data structure
    //S  : a discrete state space data structure (syslin) the regular input to
    //     output transfer
    //Sn  : a discrete state space data structure (syslin) the noise input to
    //     output transfer
    if typeof(Ar)<>"ar" then
        error(msprintf(_("%s : Wrong type for input argument #%d: An armax system expected.\n"),"arma2ss",1))
    end
    a=Ar.a;
    b=Ar.b;
    d=Ar.d;
    nu=Ar.nu;
    sig=Ar.sig;
    [ma,na]=size(a);
    nb=size(b,2);
    nd=size(d,2);

    a1=a(:,ma+1:$) //a(:,1:ma) supposed to be the identity
    if a1==[] then a1=zeros(ma,ma);na=2*ma;end

    //the input to output transfer
    A=[-a1, b(:,nu+1:$)];
    N=size(A,2);
    A=[A
    eye(na-2*ma,N)];
    B=[b(:,1:nu);
    zeros(na-2*ma,nu)]

    M=size(A,1)
    if nb-nu>0 then
        A=[A
        zeros(nu,N)
        zeros(N-M-nu,na-ma) eye(N-M-nu,nb-nu)]
        B=[B
        eye(nu,nu)
        zeros(N-M-nu,nu)]
    end
    C=eye(ma,N)
    S=syslin("d",A,B,C)

    //the noise to output transfer
    A=[-a1, d(:,ma+1:$)];
    N=size(A,2);
    A=[A
    eye(na-2*ma,N)];
    M=size(A,1)
    B=[d(:,1:ma)
    zeros(na-2*ma,ma)]
    if nd-ma>0 then
        A=[A
        zeros(ma,N)
        zeros(N-M-ma,na-ma) eye(N-M-ma,nd-ma)]
        B=[B
        eye(ma,ma)
        zeros(N-M-ma,ma)]
    end

    C=eye(ma,N)
    Sn=syslin("d",A,B*sig,C)
endfunction
