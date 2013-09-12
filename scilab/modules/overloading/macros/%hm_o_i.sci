// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function M=%hm_o_i(M,s)
    // Copyright INRIA
    //M==s
    if size(s,"*")<> 1 then
        M=%f
        return
    end
    M.entries=M.entries==s
endfunction
