// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function r=%i_j_s(a,n)
    if or(int(n)<>n)|or(n<0) then
        r=double(a).^n
    else
        if size(a,"*")==1 then
            r=iconvert(double(a).^n,inttype(a))
        else
            if size(n,"*")<>1 then
                error(43)
            else
                r=a
                for i=2:n,r=r.*a,end
            end
        end
    end
endfunction
