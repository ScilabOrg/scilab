// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function boolStatus = example_run(modulesName, functionsName, helpLanguage)

    rhs = argn(2);

    if rhs < 1 then
        modulesName = getmodules();
    end
    if rhs < 2 then
        functionsName = [];
    end
    if rhs < 3 then
        helpLanguage = "en_US";
    end

    if typeof(modulesName)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A row array of strings expected.\n"), "example_run", 1));
    end
    if size(modulesName, 2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A row array of strings expected.\n"), "example_run", 1));
    end

    if (typeof(functionsName)=="constant" & ~isempty(functionsName)) & typeof(functionsName)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A row array of strings or empty matrix expected.\n"), "example_run", 2));
    end
    if ~isempty(functionsName) & size(functionsName, 2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A row array of strings or empty matrix expected.\n"), "example_run", 2));
    end

    if typeof(helpLanguage)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "example_run", 3));
    end
    if size(helpLanguage, "*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"), "example_run", 3));
    end

    for kMod = 1:size(modulesName, "*")

        moduleName = modulesName(kMod);

        moduleDir = SCI + filesep() + "modules" + filesep() + moduleName + filesep() + "help" + filesep() + helpLanguage + filesep();

        tempModuleName = moduleName + "_help";

        workDir = TMPDIR + filesep() + tempModuleName + filesep() + "tests" + filesep() + "unit_tests" + filesep();
        if isdir(workDir) then
            rmdir(workDir, "s");
        end

        if isempty(functionsName) then
            xmlFiles = findHelpFiles(moduleDir, []);

            for kFile = 1:size(xmlFiles, "*")
                xmlFile = xmlFiles(kFile);
                createTestsFromHelp(moduleDir, workDir, xmlFile)
            end
        else
            for kFunction = 1:size(functionsName, "*")
                xmlFiles = findHelpFiles(moduleDir, functionsName(kFunction));
                if isempty(xmlFiles) then // The user wants to test a specific file
                    error(sprintf(gettext("The ""%s"" help file is not available in the ""%s"" module"), functionsName(kFunction), moduleName));
                end

                for kFile = 1:size(xmlFiles, "*")
                    xmlFile = xmlFiles(kFile);
                    createTestsFromHelp(moduleDir, workDir, xmlFile)
                end
            end
        end

        boolStatus = test_run(TMPDIR +  filesep() + tempModuleName)
    end

endfunction

function createTestsFromHelp(baseDirectory, outputDirectory, helpFile)

    mkdir(outputDirectory);

    helpFileDoc = xmlRead(helpFile);

    // Find all programlisting tag in the file
    exampleTags = xmlXPath(helpFileDoc, "//scilabhelpfile:programlisting", ["scilabhelpfile" "http://docbook.org/ns/docbook"]);

    for kExample = 1:size(exampleTags, "*")

        programListing = [];

        if exampleTags(kExample).attributes.role == "interactive-example" then
            programListing = ["// <-- INTERACTIVE TEST -->" ; programListing];
        end

        ignoredRoles = ["no-scilab-exec"];
        ignoreExample = or(exampleTags(kExample).attributes.role==ignoredRoles) | isempty(exampleTags(kExample).attributes.role);

        programListing = ["// <-- NO CHECK REF -->" ; programListing; exampleTags(kExample).content];

        if ~ignoreExample then
            functionName = fileparts(helpFile, "fname");
            relPath = dirname(getrelativefilename(baseDirectory, helpFile));

            mkdir(outputDirectory + relPath + filesep())

            if size(exampleTags, "*")>1 then
                mputl(programListing, outputDirectory + relPath + filesep() + functionName + "_" + string(kExample) + ".tst");
            else
                mputl(programListing, outputDirectory + relPath + filesep() + functionName + ".tst");
            end
        end
    end

endfunction

function xmlFilenames = findHelpFiles(directory, functionName)

    xmlFilenames = [];

    allFiles = ls(directory);

    for kFile = 1:size(allFiles, "*")
        if isfile(allFiles(kFile)) then
            if fileparts(allFiles(kFile), "extension")==".xml" then
                if isempty(functionName) then
                    xmlFilenames($+1) = allFiles(kFile);
                elseif fileparts(allFiles(kFile), "fname")==functionName then
                    xmlFilenames($+1) = allFiles(kFile);
                end
            end
        else // Directory
            xmlFilenames = [xmlFilenames;findHelpFiles(directory + allFiles(kFile) + filesep(), functionName)]
        end
    end

endfunction
