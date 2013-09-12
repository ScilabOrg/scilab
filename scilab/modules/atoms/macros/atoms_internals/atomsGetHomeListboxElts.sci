// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
// atomsGetHomeListboxElts()
//
// Returns a struct that contains the followings fields:
//  - elements("items_str")
//  - elements("items_mat")
//
// =============================================================================

function elements = atomsGetHomeListboxElts()
    items_str  = [];
    items_mat  = [];

    installed  = atomsGetInstalled();
    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    atomsfig   = findobj("tag","atomsFigure");
    allModules = atomsfig("UserData");

    items_str  = atomsSetInstalledList(installed)
    items_mat = installed(:,1)
    items_mat = [emptystr(items_mat)+"module" items_mat ]

    if items_str==[] then
        elements("items_str") = "";
    else
        elements("items_str") = items_str;
    end

    elements("items_mat") = items_mat;

endfunction
