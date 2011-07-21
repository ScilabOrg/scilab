// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Deletes the tarball used to install a package

function remList = atomsDeleteTarball(package,version)

    remList = [];

    // Check input parameters
    // =========================================================================
    rhs = argn(2);

    // Input argument number
    // =========================================================================

    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsDeleteTarball",2))
    end

    // 1st input argument
    // =========================================================================
    if type(package) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String string expected.\n"),"atomsDeleteTarball",1));
    end

    if size(package(1,:),"*") > 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",1));
    end

    // 2nd input argument
    // =========================================================================

    if type(version) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",2));
    end

    if size(version(1,:),"*") > 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDeleteTarball",2));
    end

    // Remove leading and trailing whitespace
    // =========================================================================
    package = stripblanks(package);

    //Getting the list of archives kept
    tarballs = ls(SCI+'/contrib/archives/');

    selected_package = grep(tarballs,package);
    [nr,nc]=size(selected_package)

    //Deleting archives matching name and version of the module
    for i=1:nc
        ok = grep(tarballs(selected_package(i)),'_'+version+'.');
        if ok==1 then
            deletefile(SCI+'/contrib/archives/'+tarballs(selected_package(i)));
        end
    end
endfunction
