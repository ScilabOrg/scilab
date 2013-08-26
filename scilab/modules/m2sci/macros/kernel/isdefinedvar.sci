// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [boolval,index]=isdefinedvar(var)
    // Verify if variable is defined in varslist
    // Input:
    // - var: a M2SCI tlist (funcall, operation...) or a character string (name) for special variables
    // Special variables i, j, pi, eps, nargin, nargout
    // Output:
    // - boolval:
    //   %T if input tlist is a defined 'variable' (already found while converting M-file)
    //   %F if input is not a 'variable' or an uknown 'variable'

    // Global variable for M2SCI
    global("varslist")

    // level is declared in m2sci.sci and modified in clause2sci.sci
    level;

    boolval=%F
    index=[]

    // If input is a string
    if type(var)==10 then
        if and(var<>["i","j","pi","eps","%nargin","%nargout"]) then
            warning(msprintf(gettext("Bad use of isdefinedvar() with input: %s."),var))
            return
        end
        varname=var
    else
        // If input is not a 'variable', it can be a defined variable...
        if typeof(var)=="variable" then
            varname=var.name
        elseif typeof(var)=="m2scivar" then
            varname=var.matname
        else
            return
        end
    end

    // Compare with all existing variables varslist
    levelsize=size(level,1)
    i=0
    while i<levelsize & boolval==%F
        for k=1:size(varslist)
            // Search a variable which has the same name
            if varslist(k).matname==varname then
                // Search a variable which is in the same level or in an inferior level
                if varslist(k).level==level(1:levelsize-i) then
                    boolval=%T
                    index=k
                    return
                end
            end
        end
        i=i+1
    end

endfunction
