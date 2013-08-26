// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function txt=%r_string(r)
    N=string(r.num)
    D=string(r.den)
    ln=max(matrix(length(N),2,-1),"r")
    ld=max(matrix(length(D),2,-1),"r")
    l=max(ln,ld)
    [m,n]=size(r.num);
    S=emptystr(m,n)
    for i=1:m*n
        s=2*i-1:2*i
        N(s)=part(" ",1:(l(i)-ln(i))/2)+N(s)
        D(s)=part(" ",1:(l(i)-ld(i))/2)+D(s)
        S(i) =part("-",ones(1,l(i)))
    end
    txt=emptystr(5*m,n);
    txt(1:5:$,:)=N(1:2:$,:)
    txt(2:5:$,:)=N(2:2:$,:)
    txt(3:5:$,:)=S(1:$,:)
    txt(4:5:$,:)=D(1:2:$,:)
    txt(5:5:$,:)=D(2:2:$,:)
endfunction
