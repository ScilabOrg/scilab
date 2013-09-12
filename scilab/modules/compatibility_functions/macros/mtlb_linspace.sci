// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_linspace(a,b,n)
    // Emulation function for linspace() Matlab function

    str=%F
    if type(a)==10 then
        a=asciimat(a)
        str=%T
    end
    if type(b)==10 then
        b=asciimat(b)
        str=%T
    end

    if or(type(a)==[4,6]) then
        a=bool2s(a)
    end
    if or(type(b)==[4,6]) then
        b=bool2s(b)
    end

    [lhs,rhs]=argn(0)

    if rhs==2 then
        if str then
            y=ascii(linspace(a,b))
        else
            y=linspace(a,b)
        end
    else
        if type(n)==10 then
            n=asciimat(n)
        end
        if or(type(n)==[4,6]) then
            n=bool2s(n)
        end
        if str then
            y=ascii(linspace(a,b,n))
        else
            y=linspace(a,b,n)
        end
    end
endfunction
