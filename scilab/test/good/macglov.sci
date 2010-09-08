// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [P,r]=macglov(Sl)
//[P,r]=macglov(Sl)
//Standard plant for the Glover-McFarlane problem:
// for this problem mu_optimal = 1-hankel_norm([N,M])
// with [N,M]=LCF(Sl) (Normalized coprime factorization)
// gama_optimal = 1/sqrt(mu_optimal)
//!
flag=0;Sl1=Sl(1);
if Sl1(1)=='r' then Sl=tf2ss(Sl),flag=1;end
[A,B,C,D]=Sl(2:5);[n,nb]=size(B);[nc,n]=size(C);
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
P=syslin('c',Ap,Bp,Cp,Dp);
if flag==1 then P=ss2tf(P);end
endfunction
