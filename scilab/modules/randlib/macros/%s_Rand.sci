// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which you
// should have received as part of this distribution.  The terms are also
// available at http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// Support to hypermatrix generation with grand(s1,s2,..,sn,"type_of_distribution",...)
// See bug http://bugzilla.scilab.org/5207

function r = %s_Rand(varargin)
    // Searching for the first string input parameter:
    j = 0   // its position
    for i = 1:argn(2)
        if typeof(varargin(i))=="string" & size(varargin(i),"*")==1 then
            j = i
            break
        end
    end
    if j==0 then
        msg = _("%s: a string specifying the type of distribution or the action is expected")
        error(msprintf(msg,"grand"))
    end

    // Else: the j-1 first parameters are the sizes of the hypermatrix to generate
    sizes = []
    for i = 1:j-1
        sizes = [ sizes fix(varargin(i)) ]
    end
    r = grand(1, prod(sizes), varargin(j:$))
    r = matrix(r, sizes)
endfunction
