
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1987-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 1987-2008 - INRIA - François DELEBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function[m,den]=nlev(a,z,rmax)
    //[num,den]=nlev(a,z [,rmax])  calcule (z*eye-a)**(-1) par une version
    //modifiee de l'algorithme de leverrier numeriquement plus stable.
    //
    //z     chaine de caracteres donnant le nom de la variable formelle
    //rmax  parametre optionnel utilise pour bloc diagonaliser a (voir la
    //      fonction bdiag)
    //

    rhs=argn(2);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"nlev",2,3));
    end

    z=poly(0,z);

    if rhs==3 then
        [a,x,bs]=bdiag(a,rmax),
    else
        [a,x,bs]=bdiag(a),
    end

    [m1 n1]=size(a)
    if m1<>n1 then
        error(20,1);
    end
    k=1;
    m=[];
    v=ones(1,n1);
    den=1;
    for n=bs';k1=k:k-1+n;
        //       algorithme de leverrier
        h=z*eye(n,n)-a(k1,k1)
        f=eye(n,n)
        for kl=1:n-1,
            b=h*f,
            d=-sum(diag(b))/kl,
            f=b+eye()*d,
        end
        d=sum(diag(h*f))/n
        //
        den=den*d;
        l=[1:k-1,k+n:n1] ,
        if l<>[] then
            v(l)=v(l)*d;
        end
        m=[m,x(:,k1)*f];
        k=k+n;
    end;
    m=m*diag(v)*inv(x);
endfunction



