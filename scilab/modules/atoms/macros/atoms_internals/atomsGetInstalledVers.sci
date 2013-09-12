// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Liste des versions installée de la toolbox "name"

function res = atomsGetInstalledVers(name,section)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetInstalledVers",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsGetInstalledVers",1));
    end

    if size(name,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsGetInstalledVers",1));
    end

    // Allusers/user management
    // =========================================================================

    if rhs <= 1 then
        section = "all";

    else

        // Process the 2nd input argument : section
        // Allusers can be a boolean or equal to "user" or "allusers"

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean or single string expected.\n"),"atomsGetInstalledVers",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsGetInstalledVers",2));
        end

    end

    // Get the list of installed packages
    // =========================================================================
    packages = atomsGetInstalled(section);

    // Filter on names
    // =========================================================================
    res = packages(find(packages(:,1) == name),2);

    // Descending Sort
    // =========================================================================
    res = atomsVersionSort( res , "DESC" );

endfunction
