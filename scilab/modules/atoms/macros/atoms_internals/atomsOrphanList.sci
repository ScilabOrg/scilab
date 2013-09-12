// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// display of the available toolboxes

function packages = atomsOrphanList(section)

    packages = [];

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsOrphanList",1))
    end

    // Check input argument type
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsOrphanList",1));
    end

    if and(section<>["user","allusers","all"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsOrphanList",1));
    end

    // Loop on installed packages
    // =========================================================================

    installed_list = atomsGetInstalled(section);

    for i=1:size(installed_list(:,1),"*")
        if isempty( atomsGetDepParents([installed_list(i,1) installed_list(i,2)] , section )) ..
            & (atomsGetInstalledStatus([installed_list(i,1) installed_list(i,2)],section) == "A") then
            packages = [ packages ; installed_list(i,:) ];
        end
    end

endfunction
