// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [Q]=bstap(sl)
    // Best approximant Q of Sl
    //      ||Sl-Q||    =  ||Tsl||
    //              inf
    // ||Tsl|| norm of hankel operator
    //-- sl is assumed antistable
    //-- Q : best stable approximation of Sl
    //!
    //balancing
    //-----------------------------------

    slt=gtild(sl);slt=balreal(slt);sl=gtild(slt),
    // D such that DB1'+sC1 = 0 , DD' = s**2I
    //-------------------------------------------------
    [a,b,c]=sl(2:4),[n,n]=size(a),[m,n]=size(c),
    x=-obs_gram(sl),s=x(1,1),
    [w,r]=rowcomp(x-s*eye()),r=n-r,
    b1=b(1:r,:),c1=c(:,1:r),
    [u1,s1,v1]=svd(-c1'),[u2,s2,v2]=svd(b1),
    v2=v2(:,1:m),dd=s*v1*v2',
    //
    //--------------------------------
    a22=a(r+1:n,r+1:n),
    b2=b(r+1:n,:),c2=c(:,r+1:n),
    sig=x(r+1:n,r+1:n),
    bb=-inv(s**2*eye()-sig*sig)*(sig*b2+s*c2'*dd),
    aa=-(a22+b2*bb')',cc=c2*sig+dd*b2',
    Q=syslin("c",aa,bb,cc,dd),
endfunction
