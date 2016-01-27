// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function [x,uu]=%i_gcd(p)
    //Given a polynomial vector p, [pgcd,u]=gcd(p) computes the gcd
    //of components and a unimodular matrix (with polynomial inverse) u,
    //with minimal degree such that [p1 p2]*u=[0 ... 0 pgcd]
    //!

    [lhs,rhs]=argn(0)
    [m,n]=size(p);it=inttype(p)
    mn=m*n
    p=matrix(p,1,mn)
    x=p(1);
    uu=iconvert(1,it)
    for l=2:mn,
        [x,u]=bezout(x,p(l)),
        if lhs==2 then
            uu=[uu(:,1:l-2) uu(:,l-1)*u(1,[2 1])];uu(l,l-1:l)=u(2,[2 1]);
        end
    end,
    if lhs==1 then return,end
    for l=mn:-1:2
        pivot=uu(l,l-1);
        for k=l:mn
            q=uu(l,k)/pivot
            r=uu(l,k)-q*pivot
            if q<>iconvert(0,it) then
                uu(1:l-1,k)=uu(1:l-1,k)-q*uu(1:l-1,l-1)
                uu(l,k)=r;
            end
        end
    end
endfunction
