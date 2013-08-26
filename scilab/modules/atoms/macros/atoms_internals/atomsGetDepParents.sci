// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Give the list of packages that use this package (identified by its name and version)

function packages = atomsGetDepParents(package,section)

    rhs      = argn(2);
    packages = [];

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetDepParents",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsGetDepParents",1));
    end

    if or(size(package) <> [1 2]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: 1x2 string matrix expected.\n"),"atomsGetDepParents",1));
    end

    // All user management
    // =========================================================================

    if rhs < 2 then
        section = "all";

    else

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsGetDepParents",2));
        end

        if size(section,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsGetDepParents",2));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsGetDepParents",2));
        end

    end

    if section == "all" then
        sections = ["user","allusers"];
    else
        sections = section;
    end

    for i=1:size(sections,"*")

        // Load the installed_deps struct
        // =========================================================================
        [ child_deps,parent_deps ] = atomsLoadInstalleddeps(sections(i));

        // If name - version is not a field of the struct, the job is done
        // =========================================================================

        if ~ isfield(parent_deps,package(1)+" - "+package(2)) then
            return;
        end

        // Return the matrix associated with the wanted package (name - version)
        // =========================================================================

        packages_list = parent_deps(package(1)+" - "+package(2));

        for j=1:size(packages_list,"*")
            this_package_name    = part(packages_list(j),1:regexp(packages_list(j),"/\s-\s/","o")-1);
            this_package_version = part(packages_list(j),length(this_package_name)+4:length(packages_list(j)));
            packages = [ packages ; this_package_name this_package_version sections(i) ];
        end

    end

endfunction
