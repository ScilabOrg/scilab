// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function cmap = hotcolormap(varargin)
    //graycmap   yellow to red color map.

    // Check number of input argument
    if size(varargin)<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "hotcolormap", 1));
    end
    n=varargin(1);

    // Check type of input argument
    // Check if input argument is real
    if typeof(n)<>"constant" | ~isreal(n) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a real scalar expected.\n"), "hotcolormap", 1));
    end

    // Check size of input argument
    if size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a real scalar expected.\n"), "hotcolormap", 1));
    end

    n1=fix(3/8*n);
    n2=n1
    n3=n-(n1+n2)
    //
    cmap=[(1:n1)'/n1  zeros(n1,1)  zeros(n1,1);
    ones(n2,1)  (1:n2)'/n2   zeros(n2,1);
    ones(n3,1)  ones(n3,1)   (1:n3)'/(n3)]

endfunction
