// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s=%sp_s_s(a,b)
    // %sp_s_s - subtract a scalar b to a sparse matrix a
    //!

    if size(b)==[-1 -1] then
        [m,n]=size(a)
        s=a-(b+0)*speye(m,n)
    else
        s=full(a)-b
    end
endfunction
