// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function atomsUpInstalledListbox()
    InstalledElements         = atomsGetHomeListboxElts()
    InstalledListbox          = findobj("tag","HomeListbox")
    InstalledListbox.String   = InstalledElements("items_str")
    InstalledListbox.UserData = InstalledElements("items_mat")
endfunction
