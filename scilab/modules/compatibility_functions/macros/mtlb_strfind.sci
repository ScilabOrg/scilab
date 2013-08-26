// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_strfind(A,B)
    // Emulation function for strfind() Matlab function

    if type(A)==10 & type(B)==10 then
        y=strindex(A,B)
    else
        if A==B then
            y=1
        else
            y=[]
        end
    end
endfunction
