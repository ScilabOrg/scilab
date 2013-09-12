// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x=%s_r_sp(a,b)
    // a/b , a full b sparse

    [ma,na]=size(a)
    [mb,nb]=size(b)
    if ma*na==1 then x=(a*speye(nb,nb))/b,return;end
    //if mb*nb==1 then x=a/full(b),return,end //hard coded case
    if na<>nb then error(11),end

    if mb<>nb then a=a*b';b=b*b';end

    if isreal(a)&isreal(b) then
        [h,rk]=lufact(b')
        if rk<min(mb,nb) then warning("deficient rank: rank = "+string(rk)),end
        x=[]
        for k=1:ma
            x=[x;lusolve(h,a(k,:)')']
        end
        ludel(h)
    else
        b=b';a=a'
        [h,rk]=lufact([real(b) -imag(b);imag(b) real(b)])
        if rk<2*min(mb,nb) then warning("deficient rank: rank = "+string(rk/2)),end
        x=[]
        for k=1:ma
            x=[x lusolve(h,[real(a(:,k));imag(a(:,k))])]
        end
        x=(x(1:$/2,:)+%i*x($/2+1:$,:))'
        ludel(h)
    end
endfunction
