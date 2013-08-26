// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Load one or several toolboxes

function result = atomsUpdateDep(name_parent,version_parent,name_child,version_child)

    // Init the output argument
    // =========================================================================
    result = %F;

    // Check number of input arguments
    // =========================================================================
    rhs = argn(2);
    if rhs <> 4 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsUpdateDep",2));
    end

    // Check input parameters type
    // =========================================================================

    if type(name_parent) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsUpdateDep",1));
    end

    if type(version_parent) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsUpdateDep",2));
    end

    if type(name_child) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsUpdateDep",3));
    end

    if type(version_child) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsUpdateDep",4));
    end

    // Check input parameters size
    // =========================================================================

    if size(name_parent,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsUpdateDep",1));
    end

    if size(version_parent,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsUpdateDep",2));
    end

    if size(name_child,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsUpdateDep",3));
    end

    if size(version_child,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsUpdateDep",4));
    end

    // Define the path of the loaded file
    // =========================================================================
    loaded_file = pathconvert(TMPDIR+"/atoms/loaded",%F);

    // Does the loaded file exist, otherwise return FALSE
    // =========================================================================
    if fileinfo(loaded_file) <> [] then
        loaded = mgetl(loaded_file);
    else
        return;
    end





endfunction
