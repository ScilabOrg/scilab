// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function Slmin=minss(Sl,tol)

    [lhs,rhs]=argn(0)

    if typeof(Sl)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: State-space form expected.\n"),"minss",1));
    end

    [a,b,c,d,x0,dom]=Sl(2:7)
    //observability
    if rhs<2 then tol=1.d-10*norm([a;c],1),end
    [nc,u1]=contr(a',c',tol)
    u=u1(:,1:nc)
    c=c*u;a=u'*a*u;b=u'*b,x0=u'*x0;

    //controllability
    if rhs<2 then tol=1.d-10*norm([a,b],1),end
    [no,u2]=contr(a,b,tol)
    u=u2(:,1:no)
    a=u'*a*u;b=u'*b;c=c*u

    //form the result
    Slmin=syslin(dom,a,b,c,d,u'*x0)
    //Would be nice to return U=U1*U2
endfunction
