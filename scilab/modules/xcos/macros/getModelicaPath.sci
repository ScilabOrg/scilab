// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Jérôme PICARD
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL

//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function [modelica_path, modelica_directory] = getModelicaPath()
    // Return the Modelica directory path used by Xcos diagram containing Modelica blocks
    //
    // Calling Sequence
    //  [modelica_path, modelica_directory] = getModelicaPath()
    //
    // Parameters
    //  modelica_path:          Indicates path for generic modelica blocks, user
    //                          can add paths with %MODELICA_USER_LIBS for his own
    //                          blocks.
    //  modelica_directory:     output directory for generated modelica files
    //  %MODELICA_USER_LIBS:    string path global variable used by the user to
    //                          modify modelica_path. All paths should exist.

    // Init
    modelica_path      = "SCI/modules/scicos_blocks/macros/" + ["Electrical","Hydraulics"];
    modelica_directory = pathconvert(TMPDIR+"/modelica/",%T,%T);

    // user-defined %MODELICA_USER_LIBS
    global %MODELICA_USER_LIBS
    if exists("%MODELICA_USER_LIBS") & ~isempty(%MODELICA_USER_LIBS) then

        if type(%MODELICA_USER_LIBS)<>10 then
            error(msprintf(gettext("%s: Wrong type for %s variable: A string array expected.\n"),"getModelicaPath","%MODELICA_USER_LIBS"));
        end

        if or(~isdir(%MODELICA_USER_LIBS)) then
            error(msprintf(gettext("%s: All paths defined by %s variable must exist.\n"),"getModelicaPath","%MODELICA_USER_LIBS"));
        end

        // reshape %MODELICA_USER_LIBS to get a row vector
        %MODELICA_USER_LIBS = matrix(%MODELICA_USER_LIBS,[1 prod(size(%MODELICA_USER_LIBS))])

        // Remove duplicate paths
        %MODELICA_USER_LIBS = unique(%MODELICA_USER_LIBS);

        // Concatenate %MODELICA_USER_LIBS with modelica_path
        modelica_path = [ %MODELICA_USER_LIBS modelica_path ];
    end

    // create modelica directory if it doesn't exist
    if ~isdir(modelica_directory) & mkdir(modelica_directory)<>1 then
        error(msprintf(gettext("%s: The directory %s cannot been created, please check if you have write access on this directory.\n"),"getModelicaPath",modelica_directory));
    end

endfunction

