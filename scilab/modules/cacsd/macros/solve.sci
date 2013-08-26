// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque , Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x=solve(a,b)
    // x=solve(A,b) solves A*x = b when A is an upper triagular matrix
    // made of character strings.
    //!

    [na,ma]=size(a),[mb,nb]=size(b)
    pivot=a(na,na);
    if pivot<>"1" then
        for k=1:nb,
            x(1,k)=ldivf(pivot,b(na,k))
        end
    else
        x=b(na,:)
    end
    // update
    if na>1 then
        for l=1:na-1,
            pivot=mulf("-1",a(l,na))
            for k=1:nb,
                b(l,k)=addf(b(l,k),mulf(pivot,x(k)))
            end
        end
        y=solve(a(1:na-1,1:na-1),b(1:na-1,:))
        x=[y;x]
    end
endfunction
