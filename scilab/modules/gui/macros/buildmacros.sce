// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA
// Copyright (C) 2009      - DIGITEO
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

if ~ isdef("genlib") then
    exec(SCI+"/modules/functions/scripts/buildmacros/loadgenlib.sce");
end

genlib("guilib"    ,"SCI/modules/gui/macros"     ,%f,%t);
genlib("uitreelib" ,"SCI/modules/gui/macros/tree",%f,%t);
