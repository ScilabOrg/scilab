// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [a,b1,b2,c1,c2,d11,d12,d21,d22]=smga(slp,r)
    //Matrix extractions (for use with h_inf,lft and other);
    // Utility function


    select typeof(slp)
    case "state-space" then
        p=r(2),r=r(1),
        [s1,s2,t]=size(slp);
        [a,b,c,d]=abcd(slp),
        b1=b(:,1:s2-p);
        b2=b(:,s2-p+1:s2),
        c1=c(1:s1-r,:);
        c2=c(s1-r+1:s1,:),
        d11=coeff(d(1:s1-r,1:s2-p),0),
        d12=coeff(d(1:s1-r,s2-p+1:s2),0),
        d21=coeff(d(s1-r+1:s1,1:s2-p),0),
        d22=coeff(d(s1-r+1:s1,s2-p+1:s2),0)
    case "rational" then
        [nl,nk]=size(slp);
        k1=1:nl-r(1);
        k2=nl-r(1)+1:nl;
        m1=1:nk-r(2);
        m2=nk-r(2)+1:nk;
        a=slp(k1,m1);  //p11
        b1=slp(k1,m2); //p12
        b2=slp(k2,m1); //p21
        c1=slp(k2,m2); //p22
        c2=[]
        d11=[],d12=[],d21=[],d22=[]
    end
endfunction
