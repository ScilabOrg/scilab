// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Return the mlist associated with the wanted toolbox

function details = atomsToolboxDetails(package,field)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsToolboxDetails",1,2));
    end

    // Check input parameters type
    // =========================================================================

    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsToolboxDetails",1));
    end

    if rhs>1 & type(field)<>10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsToolboxDetails",2));
    end

    // Check input parameters dimensions
    // =========================================================================

    if or(size(package) <> [1 2]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: 1x2 string matrix expected.\n"),"atomsToolboxDetails",1));
    end

    if rhs>1 & size(field,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsToolboxDetails",2));
    end

    // Get the full list of packages
    // =========================================================================

    allpackages = atomsDESCRIPTIONget();

    name    = package(1);
    version = package(2);

    if ~ isfield(allpackages,name) then
        error(msprintf(gettext("%s: the package ''%s'' does not exist.\n"),"atomsToolboxDetails",name));
    end

    package_versions = allpackages(name);

    if ~ isfield(package_versions,version) then
        error(msprintf(gettext("%s: the package ''%s'' does not exist.\n"),"atomsToolboxDetails",name+" - "+version));
    end

    details = package_versions(version);

    // A field is gived :
    // =========================================================================

    if rhs>=2 then

        if ~ isfield(details,field) then
            error(msprintf(gettext("%s: the package ''%s'' does not contain the field ''%s''.\n"),"atomsToolboxDetails",name+" - "+version,field));
        end

        details = details(field);

        return;
    end

    // Check if this version is installed
    // =========================================================================

    if atomsIsInstalled([name version]) then

        installedDetails          = atomsGetInstalledDetails(package);
        details("Version")        = installedDetails(2);
        details("installAlluser") = installedDetails(3);
        details("installPath")    = installedDetails(4);
        details("installStatus")  = installedDetails(5);

    end

endfunction
