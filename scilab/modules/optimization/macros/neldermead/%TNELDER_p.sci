// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// %TNELDER_p --
//   Prints the string containing the nelder mead component.
//
function %TNELDER_p ( this )
    str = string ( this )
    srows = size(str,"r")
    for i = 1 : srows
        mprintf("%s\n",str(i))
    end
endfunction


