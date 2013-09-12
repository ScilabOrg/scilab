// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%spb_e(i,j,a)
    // r=a(i,j) for f sparse in some special cases
    //!

    [lhs,rhs]=argn(0)
    if rhs==2 then
        a=j;
        a=a(:)
        r=a(i)
    end
endfunction
