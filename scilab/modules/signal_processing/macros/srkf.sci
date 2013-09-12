// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x1,p1]=srkf(y,x0,p0,f,h,q,r)
    //square root kalman filter algorithm
    //Input to the macro is:
    // f,h    :current system matrices
    // q,r    :covariance matrices of dynamics
    //        :and observation noise
    // x0,p0  :state estimate and error variance
    //        :at t=0 based on data up to t=-1
    // y      :current observation
    //
    //Output from the macro is:
    // x1,p1  :updated estimate and error covariance
    //        :at t=1 based on data up to t=0
    //!

    n=max(size(x0));
    p=max(size(y));

    j=[chol(r)',0*r];
    g=[0*q,chol(q)'];

    mat=[h*p0,j;f*p0,g];
    [q,tmat]=qr(mat')';
    p1=tmat(p+1:p+n,p+1:p+n);
    k=tmat(p+1:p+n,1:p);
    re=tmat(1:p,1:p);

    epsilon=y-h*x0;
    x1=f*x0+k*(re**(-1))*epsilon;
endfunction
