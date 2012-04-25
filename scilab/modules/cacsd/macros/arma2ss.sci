// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
  [ma,na]=size(a);
  nb=size(b,2);
  nd=size(d,2);
  
  //Create the regular input to output system
  A=[-a(:,ma+1:$) b(:,nu+1:$)];
  B=b(:,1:nu);

  nA=size(A,2);//na-ma+nb-nu
  if na-ma>0 then
    n=na-2*ma;
    A=[A
       eye(n,n), zeros(n,nA-n)];
    B=[B;
       zeros(n,nu)]
  end
  if nb>nu then
    n=nb-2*nu;
    A=[A;
       zeros(nu,nA);
       zeros(n,na-ma),eye(n,n),zeros(n,nA-(na-ma+n))];
    B=[B;
       eye(nu,nu)
       zeros(n,nu)]
  end
  C=eye(ma,nA)
  S=syslin('d',A,B,C)
  
  //Create the noise input to output system
  Sn=[]
  if argn(1)>1&d<>[] then
    An=[-a(:,ma+1:$) d(:,ma+1:$)];
    Bn=d(:,1:ma);
    nA=size(An,2);
    if na-ma>0 then
      n=na-2*ma;
      An=[An
          eye(n,n),zeros(n,nA-n)];
      Bn=[Bn;
          zeros(n,ma)]
    end

    if nd>ma then
      n=nd-2*ma;
      An=[An
          zeros(ma,nA)
          zeros(n,na-ma+nb-nu), eye(n,n), zeros(n,nA-(na-ma+n))];
      Bn=[Bn;
          eye(ma,ma)
          zeros(n,ma)]
    end
    Cn=eye(ma,nA)
    Sn=syslin('d',An,Bn*Ar.sig,Cn)
  end
endfunction
