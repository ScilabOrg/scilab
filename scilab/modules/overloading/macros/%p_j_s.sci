// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%p_j_s(p,s)
    // %p_j_s(p,s)  computes p.^s for p polynomial matrix in special cases
    //!

    if s==[] then f=[],return,end
    if  or(imag(s)<>0)|or(int(s)<>s) then error(msprintf(_("%s: Wrong type for input argument #%d: An integer matrix expected.\n"),"%p_j_s",2)),end
    [m,n]=size(p)
    [ms,ns]=size(s)
    if ms==1&ns==1 then
        if s<0 then
            if or(abs(coeff(p(:)))*ones(max(degree(p))+1,1)==0) then
                error(27)
            end
            f=rlist(ones(p),p.^(-s),[])
        else // this case is in fact hard coded
            f=p.^s
        end
    elseif m==1&n==1 then // Element wise exponentiation p.^s with p "scalar"
        kp=find(s>=0)
        kn=find(s<0)
        num=ones(s)
        den=ones(s)
        num(kp)=p.^s(kp)
        p=1/p
        num(kn)=p(2).^(-s(kn))
        den(kn)=p(3).^(-s(kn))
        f=rlist(num,den,[])
    elseif ms==m&ns==n then  // Element wise exponentiation
        p=p(:);s=s(:);
        kp=find(s>=0)
        kn=find(s<0)
        num=p
        den=ones(s)
        num(kp)=num(kp).^s(kp)
        if or(abs(coeff(p(kn)))*ones(max(degree(p(kn)))+1,1)==0) then
            error(27)
        end
        num(kn)=ones(p(kn))
        den(kn)=p(kn).^(-s(kn))
        f = rlist(matrix(num,n,m),matrix(den,n,m),[])
    else
        error(30)
    end

endfunction
