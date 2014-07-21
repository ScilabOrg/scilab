// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function vcPath64 = dlwGet64BitPath();
    vcPath64 = [];
    if win64() then
        MSCompiler = findmsvccompiler();
        if (MSCompiler == 'msvc120express' | MSCompiler == 'msvc110express' | MSCompiler == 'msvc100express' | MSCompiler == 'msvc90express') then
            programFilesx86Path = getenv('ProgramFiles(x86)', '');
            if (programFilesx86Path <> '') then
                if isfile(programFilesx86Path + '\Microsoft Visual Studio 12.0\VC\bin\cl.exe') then
                    vcPath64 = programFilesx86Path + '\Microsoft Visual Studio 12.0';
                    return
                end

                if isfile(programFilesx86Path + '\Microsoft Visual Studio 11.0\VC\bin\cl.exe') then
                    vcPath64 = programFilesx86Path + '\Microsoft Visual Studio 11.0';
                    return
                end
                
                if isfile(programFilesx86Path + '\Microsoft Visual Studio 10.0\VC\bin\amd64\cl.exe') then
                    vcPath64 = programFilesx86Path + '\Microsoft Visual Studio 10.0';
                    return
                end
                
                if isfile(programFilesx86Path + '\Microsoft Visual Studio 9.0\VC\bin\amd64\cl.exe') then
                    vcPath64 = programFilesx86Path + '\Microsoft Visual Studio 9.0';
                end
            end
        else
            select MSCompiler,
            // Microsoft Visual 2013 Studio Professional
            case  'msvc120pro' then
                vcPath64 = dlwGetVc12ProPath(); //same as VS2012
            // Microsoft Visual 2012 Studio Professional
            case  'msvc110pro' then
                vcPath64 = dlwGetVc11ProPath();
            // Microsoft Visual 2010 Studio Professional
            case  'msvc100pro' then
                vcPath64 = dlwGetVc10ProPath();
            // Microsoft Visual 2008 Studio Professional
            case  "msvc90pro" then
                vcPath64 = dlwGetVc90ProPath();
            // Microsoft Visual 2008 Studio Standard
            case  "msvc90std" then
                vcPath64 = dlwGetVc90StdPath();
            else
                vcPath64 = [];
            end
        end
    end
endfunction
//=============================================================================
