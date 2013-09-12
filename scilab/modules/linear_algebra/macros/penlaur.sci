
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1988-2008 - INRIA - François DELEBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [Si,Pi,Di,order]=penlaur(E,A)
    //[Si,Pi,Di,order]=penlaur(E,A)
    // First Laurent coefficients of (s*E-A)^-1;
    // (s*E-A)^-1 = ... + Si/s - (Pi + s*Di + ... +s^order Ni) at s = infinity
    // order = order of the singularity
    // The matrix s*E-A should be invertible.
    // Experimental version: troubles when bad conditioning of
    // (so*E-A)...)
    //!
    [LHS,RHS]=argn(0);
    if RHS==1 then [E,A]=pen2ea(E);end
    seed=rand("seed");typ=rand("info");
    rand("normal");rand("seed",0);
    tests=rand(1,10);
    conditions=0*tests;k=1;
    for s0=tests, conditions(k)=cond(s0*E-A);k=k+1;end
    [w,k1]=min(conditions);
    rand(typ);rand("seed",seed)
    if w>1.d+20 then
        error(msprintf(gettext("%s: Singular pencil."),"penlaur"));
        return;
    end
    s0=tests(k1);
    J=inv(s0*E-A);
    [Se,Pe,De,i1]=projspec(J*E);
    [Sa,Pa,Da,i2]=projspec(J*A);
    order=i1-1;
    Si=Se*J;
    Pi=Pe*Sa*J;
    Di=Pi*E*Pi;
    if order==0 then Di=0*Di;end
    //[S,P,D,index]=projspec(Di*E);
endfunction
