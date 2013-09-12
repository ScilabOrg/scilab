//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//
function compilerpath = getmodelicacpath()

    MODELICAC_FILENAME = "modelicac";

    compilerpath = "";

    if getos() == "Windows" then
        // --- WINDOWS ---
        MODELICAC_FILENAME = MODELICAC_FILENAME + ".exe";
        defaultPath = SCI + "/bin/";

        if isfile(defaultPath + MODELICAC_FILENAME) then
            compilerpath = defaultPath;
        else
            compilerpath = "";
        end

    else
        // --- LINUX/MACOS ---

        // Check if source tree version
        if isdir(SCI+"/modules/core/includes/") then
            defaultPath = SCI + "/modules/scicos/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

        // Check if binary version
        if compilerpath=="" & isdir(SCI+"/../../include/scilab/") then
            defaultPath = SCI + "/../../bin/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

        // Check if system version
        if compilerpath=="" & isdir("/usr/include/scilab/") then
            defaultPath = "/usr/bin/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

    end
endfunction

