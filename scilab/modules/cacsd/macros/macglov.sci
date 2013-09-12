// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [P,r]=macglov(Sl)
    //[P,r]=macglov(Sl)
    //Standard plant for the Glover-McFarlane problem:
    // for this problem mu_optimal = 1-hankel_norm([N,M])
    // with [N,M]=LCF(Sl) (Normalized coprime factorization)
    // gama_optimal = 1/sqrt(mu_optimal)
    //!
    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),..
        "macglov",1))
    end

    if and(typeof(Sl)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"macglov",1))
    end
    if Sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"macglov",1))
    end
    flag=0;
    if typeof(Sl)=="rational" then Sl=tf2ss(Sl),flag=1;end
    [A,B,C,D]=abcd(Sl);[n,nb]=size(B);[nc,n]=size(C);
    r=size(D);
    [Z,H]=gfare(Sl);
    R1=eye()+D*D';
    R12=sqrtm(R1);
    Ap=A;
    Bp=[-H*R12,B];
    Cp=[C;0*ones(nb,n);C];
    Dp=[R12,0*C*B;
    0*ones(nb,nc),eye(nb,nb);
    R12,D];
    P=syslin("c",Ap,Bp,Cp,Dp);
    if flag==1 then P=ss2tf(P);end
endfunction
