//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function block_parameter_error(inform, expected)
// Displays an Xcos parameter error box.
// Calling Sequence
// block_parameter_error(inform, expected)
// Parameters
// inform: a string, type of the error
// expected: a string, expected value
// Description
// This function displays an error message box with two lines. The first line
// (<emphasis role="bold">inform</emphasis>) parameter is a precise description
// of the error. The second line (<emphasis role="bold">expected</emphasis> parameter)
// is an explanation on the expected value for the user.
// Examples
// block_parameter_error(msprintf("Wrong values for ''Port Number'' parameter: %d."), port), ..
// "Strictly positive integer expected.");
// Authors
// Bernard DUJARDIN

    rhs=argn(2)
    if rhs <> 2 then
       error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "block_parameter_error", 2))
    end
    if typeof(inform) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "block_parameter_error", 1 ))
    end
    if typeof(expected) <> "string"
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"block_parameter_error", 2));
    end

    boxmsg = [" "; inform; " "; expected];
    messagebox(boxmsg, gettext("Xcos block parameter error"), "error","modal");
endfunction
