// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

// Return the sorted version list of the package entered as input argument

function versions = atomsGetVersions(name,min_version,max_version,min_version_included,max_version_included)

    // Initialize
    // =========================================================================
    versions = [];

    // Check input parameters
    // =========================================================================

    rhs  = argn(2);

    if rhs < 1 | rhs > 5 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsGetVersions",1,5));
    end

    if type(name) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",1));
    end

    if (rhs>1) & (type(min_version)<>10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",2));
    end

    if (rhs>2) & (type(max_version)<>10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"atomsGetVersions",3));
    end

    if (rhs>3) & (type(min_version_included)<>4) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean expected.\n"),"atomsGetVersions",4));
    end

    if (rhs>4) & (type(max_version_included)<>4) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean expected.\n"),"atomsGetVersions",5));
    end

    // Default values
    // =========================================================================

    if (rhs<2) | ( (rhs>=2) & (min_version=="") ) then
        min_version = "0";
    end

    if (rhs<3) | ( (rhs>=3) & (max_version=="") ) then
        max_version = "9999999";
    end

    if rhs<4 then
        min_version_included = %T;
    end

    if rhs<5 then
        max_version_included = %T;
    end

    // Get all package description
    // =========================================================================
    allpackages = atomsDESCRIPTIONget();

    // Check if it's a Atoms package
    // =========================================================================
    if ~isfield(allpackages, name) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ""%s"" must be an available atoms package name.\n"), "atomsGetVersions",1, name));
    end

    // Get versions
    // =========================================================================
    package_versions          = allpackages(name);
    package_versions_tab      = getfield(1,package_versions);
    package_versions_tab(1:2) = [];
    package_versions_tab      = package_versions_tab';

    without_packaging_ver     = [];

    for i=1:size(package_versions_tab,"*")
        if ~ isempty(strindex(package_versions_tab(i),"-")) then
            without_packaging_ver = [ without_packaging_ver ; part(package_versions_tab(i),1:strindex(package_versions_tab(i),"-")-1) ];
        else
            without_packaging_ver = [ without_packaging_ver ; package_versions_tab(i) ];
        end
    end

    // Delete out of bounds versions
    // =========================================================================

    if isempty(strindex(max_version,"-")) then
        // max_version doesn't contain a packaging version
        if max_version_included then
            indexToDel = find(atomsVersionCompare(without_packaging_ver,max_version) >  0);
        else
            indexToDel = find(atomsVersionCompare(without_packaging_ver,max_version) >= 0);
        end
    else
        // max_version contains a packaging version
        if max_version_included then
            indexToDel = find(atomsVersionCompare(package_versions_tab,max_version) >  0);
        else
            indexToDel = find(atomsVersionCompare(package_versions_tab,max_version) >= 0);
        end
    end

    package_versions_tab(indexToDel,:) = [];
    without_packaging_ver(indexToDel)  = [];

    if isempty(strindex(min_version,"-")) then
        // min_version doesn't contain a packaging version
        if min_version_included then
            indexToDel = find(atomsVersionCompare(without_packaging_ver,min_version) <  0)
        else
            indexToDel = find(atomsVersionCompare(without_packaging_ver,min_version) <= 0)
        end
    else
        // min_version contains a packaging version
        if min_version_included then
            indexToDel = find(atomsVersionCompare(package_versions_tab,min_version) <  0)
        else
            indexToDel = find(atomsVersionCompare(package_versions_tab,min_version) <= 0)
        end
    end

    package_versions_tab(indexToDel,:) = [];
    without_packaging_ver(indexToDel)  = [];

    // Sort the version matrix
    // =========================================================================
    versions = atomsVersionSort(package_versions_tab,"DESC");

endfunction
