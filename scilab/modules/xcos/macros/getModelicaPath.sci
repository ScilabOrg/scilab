// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Jérôme PICARD
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL

//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// return the Modelica directory path modelica_path
// used by Xcos diagram containing Modelica blocks

function [modelica_path, modelica_directory] = getModelicaPath()

    // modelica_path:
    //  - indicates path for generic modelica blocks
    //  - this variable can be defined by the user for his own blocks

    // Init
    modelica_path = "SCI/modules/scicos_blocks/macros/" + ["Electrical","Hydraulics"];

    // user-defined %MODELICA_PATH
    if exists("%MODELICA_PATH") then

        if type(%MODELICA_PATH)<>10 then
            error(msprintf(gettext("%s: wrong type for %s variable: A string array expected\n"),"getModelicaPath","%MODELICA_PATH"));
        end

        if or(~isdir(%MODELICA_PATH)) then
            error(msprintf(gettext("%s: All paths defined by %s variable should exist\n"),"getModelicaPath","%MODELICA_PATH"));
        end

        // reshape %MODELICA_PATH to get a row vector
        %MODELICA_PATH = matrix(%MODELICA_PATH,[1 prod(size(%MODELICA_PATH))])

        // Remove duplicates paths
        %MODELICA_PATH = unique(%MODELICA_PATH);

        // Concatenate %MODELICA_PATH with modelica_path
        modelica_path = [ %MODELICA_PATH modelica_path ];
    end

    // modelica_directory:
    //  - Path for generic modelica blocks
    //  - Add TMPDIR/modelica for generic modelica blocks
    //    needed by modelicat to compile every modelica file
    modelica_directory = pathconvert(TMPDIR+"/modelica/",%T,%T);

    // create modelica directory if it doesn't exist
    if ~isdir(modelica_directory) & mkdir(modelica_directory)<>1 then
        error(msprintf(gettext("%s: The directory %s cannot be created\n"),"getModelicaPath",modelica_directory));
    end

endfunction
