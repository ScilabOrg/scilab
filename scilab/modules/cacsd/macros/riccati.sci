// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x1,x2]=riccati(a,b,c,dom,typ)
    //[x1,[x2]]=riccati(a,b,c,dom,[typ]) is a Riccati solver
    // x=x1/x2 solves:
    // a'*x+x*a-x*b*x+c=0 (continuous time case)
    // a'*x*a-(a'*x*b1/(b2+b1'*x*b1))*(b1'*x*a)+c-x with b=b1/b2*b1'
    // (discrete time case)
    // If called with LHS=1 (one output argument) riccati returns x.
    //
    // -- a,b,c real matrices nxn, b and c symmetric.
    // -- dom = 'c' or 'd' for the time domain (continuous or discrete)
    // -- typ = 'eigen' --->block diagonalization
    //    typ = 'schur' --->schur method
    // See also ric_desc
    //!

    [lhs,rhs]=argn(0),
    if rhs==4 then typ="eigen",end,
    ham=[a -b;-c -a'],
    [n,n]=size(a),
    if part(dom,1)=="c" then
        select  typ,
        case "schur" then
            [s,d]=schur(ham,"c"),
            if d<>n then
                error(msprintf(gettext("%s: Wrong dimension (%d) of stable subspace: %d expected.\n"),"riccati",d, n))
            end
            s=s(:,1:n),
        case "eigen" then
            [hb,u1]=bdiag(ham),
            [u2,d]=schur(hb,"c"),
            u=u1*u2,
            if d<>n then
                error(msprintf(gettext("%s: Wrong dimension (%d) of stable subspace: %d expected.\n"),"riccati",d, n))
            end
            s=u(:,1:n),
        end,
    else
        aa=[eye(n,n) b;0*ones(n,n) a'],bb=[a  0*ones(n,n);-c eye(n,n)],
        [bs,as,s,n1]=schur(bb,aa,"d");
        if n1<>n then
            error(msprintf(gettext("%s: Wrong dimension (%d) of stable subspace: %d expected.\n"),"riccati",n1, n))
        end
        s=s(:,1:n1);
    end,
    x1=s(n+1:2*n,:),x2=s(1:n,:),
    if lhs==1 then x1=x1/x2,end
endfunction
