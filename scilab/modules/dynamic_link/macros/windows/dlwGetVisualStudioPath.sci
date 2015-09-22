// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function VSPath = dlwGetVisualStudioPath()
    msvc = findmsvccompiler();

    VSPath = "";
    select msvc
        // Microsoft Visual 2013 Studio Professional
    case  "msvc120pro" then
        VSPath = dlwGetVc12ProPath();
        // Microsoft Visual 2013 Express
    case  "msvc120express" then
        VSPath = dlwGetVc12ExpressPath();
        // Microsoft Visual 2012 Studio Professional
    case  "msvc110pro" then
        VSPath = dlwGetVc11ProPath();
        // Microsoft Visual 2012 Express
    case  "msvc110express" then
        VSPath = dlwGetVc11ExpressPath();
        // Microsoft Visual 2010 Studio Professional
    case  "msvc100pro" then
        VSPath = dlwGetVc10ProPath();
        // Microsoft Visual 2010 Express
    case  "msvc100express" then
        VSPath = dlwGetVc10ExpressPath();
        // Microsoft Visual 2008 Studio Professional
    case  "msvc90pro" then
        VSPath = dlwGetVc90ProPath();
        // Microsoft Visual 2008 Studio Standard
    case  "msvc90std" then
        VSPath = dlwGetVc90StdPath();
        // Microsoft Visual 2008 Express
    case  "msvc90express" then
        VSPath = dlwGetVc90ExpressPath();
    else
    end
endfunction
//=============================================================================
