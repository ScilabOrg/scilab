// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Give the list of packages needed by this package (identified by its name and version)

function packages = atomsGetDepChilds(package,section)

    rhs      = argn(2);
    packages = [];

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetDepChilds",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepChilds",1));
    end

    if or(size(package) <> [1 2]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: 1x2 string matrix expected.\n"),"atomsGetDepChilds",1));
    end

    // All user management
    // =========================================================================

    if rhs < 2 then
        section = "all";

    else

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsGetDepChilds",2));
        end

        if size(section,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsGetDepChilds",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsGetDepChilds",2));
        end

    end

    // Load the installed_deps struct
    // =========================================================================
    installed_deps = atomsLoadInstalleddeps(section);

    // If name - version is not a field of the struct, the job is done
    // =========================================================================

    if ~ isfield(installed_deps,package(1)+" - "+package(2)) then
        return;
    end

    // Return the matrix associated with the wanted package (name - version)
    // =========================================================================

    packages_list = installed_deps(package(1)+" - "+package(2));

    for i=1:size(packages_list,"*")
        this_package_name    = part(packages_list(i),1:regexp(packages_list(i),"/\s-\s/","o")-1);
        this_package_version = part(packages_list(i),length(this_package_name)+4:length(packages_list(i)));
        packages = [ packages ; this_package_name this_package_version ];
    end

endfunction
