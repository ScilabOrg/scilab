// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function boolStatus = help_run(modulesName, functionName, helpLanguage)

    rhs = argn(2);

    if rhs < 1 then
        modulesName = getmodules();
        functionName = "*";
        helpLanguage = getlanguage();
    end
    if rhs == 1 then
        functionName = "*";
        helpLanguage = getlanguage();
    end
    if rhs == 2 then
        helpLanguage = getlanguage();
    end

    for kMod = 1:size(modulesName, "*")

        moduleName = modulesName(kMod);

        moduleDir = SCI + "/modules/" + moduleName + "/help/" + helpLanguage + "/";

        xmlFiles = ls(moduleDir + functionName + ".xml");

        for kFile = 1:size(xmlFiles, "*")
            xmlFile = xmlFiles(kFile);

            functionName = fileparts(xmlFile, "fname");

            doc = xmlRead(xmlFile);

            // Find all programlisting tag in the file
            xp = xmlXPath(doc, "//foo:programlisting", ["foo" "http://docbook.org/ns/docbook"]);

            workDir = TMPDIR + "/" + moduleName + "/help/" + helpLanguage + "/tests/unit_tests/";

            if isdir(workDir) then
                rmdir(workDir);
            end

            mkdir(workDir);

            for kProg = 1:size(xp, "*")

                programListing = [];

                if xp(kProg).attributes.role == "interactive-example" then
                    programListing = ["// <-- INTERACTIVE TEST -->" ; programListing];
                end

                ignoreExample = %F
                if xp(kProg).attributes.role == "no-scilab-exec" then
                    ignoreExample = %T;
                end
                if isempty(xp(kProg).attributes.role) then
                    ignoreExample = %T;
                end

                programListing = ["// <-- NO CHECK REF -->" ; programListing; xp(kProg).content];

                if ~ignoreExample then
                    if size(xp, "*")>1 then
                        mputl(programListing, workDir + functionName + "_" + string(kProg) + ".tst");
                    else
                        mputl(programListing, workDir + functionName + ".tst");
                    end
                end
            end
        end
        oldPwd = pwd();
        cd(TMPDIR)
        boolStatus = test_run(moduleName + "/help/" + helpLanguage + "/")
        cd(oldPwd)
    end

endfunction
