// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [h,err]=time_id(n,u,y)

    [lhs,rhs]=argn(0)
    y=y(:)
    npt=size(y,"*");
    select type(u)
    case 1 then
        u=u(:)
    case 10 then
        select part(u,1)
        case "i" then
            u=eye(npt,1)
        case "s" then
            u=ones(npt,1)
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"time_id",2,"""i"",""s"""))
        end
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"time_id",2,"""i"",""s"""))
    end
    if y(1)==0 then // strictly proper case
        m(npt-1,2*n)=0;
        for k=1:n,m(k:npt-1,[k k+n])=[-y(1:npt-k) u(1:npt-k)];end
        coef=m\y(2:npt);
        num=poly(coef(2*n:-1:n+1),"z","c");
        den=poly([coef(n:-1:1);1],"z","c");
    else
        m(npt,2*n+2)=0;
        for k=1:n+1,m(k:npt,[k k+n+1])=[-y(1:npt-k+1) u(1:npt-k+1)];end
        coef=-m(:,2:$)\m(:,1)
        num=poly(coef(2*n+1:-1:n+1),"z","c");
        den=poly([coef(n:-1:1);1],"z","c");
    end

    h=syslin("d",num,den)

    if lhs==2 then
        err=norm(y-rtitr(num,den,u')',2)
    end
endfunction
