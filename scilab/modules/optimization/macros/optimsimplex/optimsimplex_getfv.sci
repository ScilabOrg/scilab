// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// optimsimplex_getfv --
//   Returns the function value at given index
// Arguments
//   ive : vertex index
//
function fv = optimsimplex_getfv ( this , ive )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for first argument: TSIMPLEX expected.\n"), "optimsimplex_getfv"));
    end
    if type(ive) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "optimsimplex_getfv", 2));
    end
    if or(size(ive) <> [1 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: (%d,%d) expected.\n"), "optimsimplex_getfv", 2, 1, 1));
    end
    if ive-floor(ive) <> 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Integer expected.\n"), "optimsimplex_getfv", 2));
    end
    fv = this.fv ( ive , 1 )
endfunction
