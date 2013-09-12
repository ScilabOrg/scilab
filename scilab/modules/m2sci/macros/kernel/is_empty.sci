// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=is_empty(A)
    // M2SCI function
    // Checks if all A dimension are 0
    // Input: A = a M2SCI tlist
    // Output: r = boolean value (true if A is an empty matrix)

    n=size(A.dims)
    nb=0
    for k=1:n
        if A.dims(k)==0 then
            nb=nb+1
        end
    end

    r=nb==n
endfunction
