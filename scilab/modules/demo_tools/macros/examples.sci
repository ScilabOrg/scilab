//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- ENGLISH IMPOSED -->

function [path] = examples(varargin)

    [lhs, rhs]=argn()

    if (rhs == 1) then
        f = varargin(1)
        // Check type
        if (typeof(f) <> "string")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"),"examples", 1));
        end
        viewCode = %f;
    elseif (rhs == 2) then
        f = varargin(1)
        exIndex = varargin(2)
        // Check type
        if (typeof(f) <> "string")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"),"examples", 1));
        end
        if (typeof(exIndex) <> "constant" & typeof(exIndex) <> "boolean")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer or a boolean expected.\n"),"examples", 2));
        end
        if (typoeof(exIndex) == "constant") then
            // Check positive
            if (exIndex <= 0) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: strictly positive expected.\n"),"examples", 2));
            end
            // Check integer
            if (floor(exIndex) <> exIndex) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"examples", 2));
            end
            viewCode = %f;
        end
    elseif (rhs == 3) then
        f = varargin(1)
        exIndex = varargin(2)
        viewCode = varargin(3)
        // Check type
        if (typeof(f) <> "string")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"examples", 1));
        end
        if (typeof(exIndex) <> "constant")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"index", 2));
        end
        // Check positive
        if (exIndex <= 0) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: strictly positive expected.\n"),"examples", 2));
        end
        // Check integer
        if (floor(exIndex) <> exIndex) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: an integer expected.\n"),"examples", 2));
        end
        if (typeof(viewCode) <> "boolean")
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a boolean expected.\n"),"examples", 3));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"),"examples", 1, 2, 3));
    end

    // Try to find the function
    // macros: whereis(), built-in functions (module): funptr(), keywords: getscilabkeywords()
    macroPath = get_function_path(f);
    builtIn = floor(funptr(f) / 1000);
    if (macroPath == []) then
        if (builtIn == 0) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: function does not exist in Scilab.\n"),"examples", 1));
        else
            // From the 'module number', find the .dem script file
        end
    else
        // From the 'macro path', find the .dem script file
    end

    //if (!exIndexExistsForFunction) then
    //    error(msprintf(gettext("%s: Wrong value for input argument #%d: no example with that index.\n"),"examples", 2));
    //else
    //    Run the example number exIndex
    //end

    if (viewCode == %t) then
        // View / edit the code
    end

    //path = path to the function;

endfunction

