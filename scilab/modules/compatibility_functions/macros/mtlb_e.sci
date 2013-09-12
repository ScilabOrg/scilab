// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=mtlb_e(a,i)
    if type(a)==10 then
        a=strcat(a);
        if i==$ then
            r=part(a,length(a))
        else
            r=part(a,i)
        end
    else
        r=a(i)
        if size(i,2)>1 then //index is a row vector
            if min(size(a))>1 then // a is'nt a vector
                r=matrix(r,1,size(r,"*")) //return a row vector
            end
        end
    end
endfunction
