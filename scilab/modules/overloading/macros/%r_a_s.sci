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

function f=%r_a_s(f,m)
    //f = f+m,
    //f: transfer matrix, m : scalar or scalar matrix
    //author Serge Steer INRIA
    //!
    [num,den]=f(["num","den"])
    szf=size(den)
    szm=size(m)

    if isempty(m)
        f = [];
        return
    end

    if and(szf>=0)&and(szm>=0) then
        if prod(szf)==1&prod(szm)>1 then
            den=den(ones(m))
            szf=szm
        end

        if size(szf,"*")>2 then
            num=num(:);den=den(:);m=m(:)
        end
        [num,den]=simp(num+m.*den,den)
        num=matrix(num,szf)
        den=matrix(den,szf)
    else
        //at leat one matrix is eye*x
        if size(szf,"*")>2|size(szm,"*")>2 then
            error(8)
        end
        if or(szf<0)&or(szm<0) then
            [num,den]=simp(num+m.*den,den)
        elseif or(szf<0) then
            [num,den]=simp(num+m.*den,den*ones(m))
        elseif or(szm<0) then
            [num,den]=simp(num+(m+0)*eye(den).*den,den)
        end
    end
    f=rlist(num,den,f.dt)
endfunction
