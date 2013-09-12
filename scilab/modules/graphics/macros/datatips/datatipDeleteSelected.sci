// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatipDeleteSelected(curve_handles)
    //datatip utility function
    //set the selected datatip(s)

    [curve,ind]=datatipGetSelected(curve_handles)
    if ind<>[] then
        datatipRemove(curve,ind)
    end
endfunction

