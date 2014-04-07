// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - F.Delebecque
// Copyright (C) INRIA - 1996 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [ze,po,gain]=zpell(epsilon,A,omegac,omegar)
    //[ze,po,gain]=zpell(epsilon,A,omegac,omegar)
    //Poles and zeros of prototype lowpass elliptic filter
    //gain is the gain of the filter
    //  epsilon :Ripple of filter in pass band (0<epsilon<1)
    //  A       :Attenuation of filter in stop band (A>1)
    //  omegac  :Pass band cut-off frequency in rd/s
    //  omegar  :Stop band cut-off frequency in rd/s
    //  ze      :Resulting zeros of filter
    //  po      :Resulting poles of filter
    //  gain    :Resulting gain of filter
    //
    //!

    m1=(epsilon*epsilon)/(A*A-1);
    K1=delip(1,sqrt(m1));
    K1t=imag(delip(1/sqrt(m1),sqrt(m1)));
    m=(omegac/omegar)^2;
    K=delip(1,sqrt(m));
    Kt=imag(delip(1/sqrt(m),sqrt(m)));
    n=(K1t*K)/(K1*Kt);
    order=round(n);
    u0=-(Kt/K1t)*delip(sqrt(1/(1+epsilon*epsilon)),sqrt(1-m1));
    even=2*int(order/2);
    if order<>even then,
        vmin=2*K/n;
    else,
        vmin=K/n;
    end,
    v=vmin:(2*K/n):K;
    un=ones(1:max(size(v)));
    zlambda=-un*Kt+%i*v;
    plambda= u0*un+%i*v;
    ze=%i*imag(%i*omegac*%sn(-%i*zlambda,m));
    ze=[ze,conj(ze)];
    po=%i*omegac*%sn(-%i*plambda,m);
    po=[po,conj(po)];
    if order<>even then,
        po=[po,%i*omegac*%sn(-%i*u0,m)];
    end,
    gain=abs(real(prod(po))/real(prod(ze)));
    if order==even then,
        gain=gain/sqrt(1+epsilon^2);
    end;
endfunction
