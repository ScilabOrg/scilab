// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

mode(-1);

// Create test files for M2SCI

filePath = get_absolute_file_path("createTestFiles.sce");

mfilesPath = "SCI/modules/m2sci/tests/unit_tests/mfiles/";
mfilesNoexecPath = "SCI/modules/m2sci/tests/unit_tests/mfilesnoexec/";
conversionTestsPath = "SCI/modules/m2sci/tests/unit_tests/conversion/";
executionTestsPath = "SCI/modules/m2sci/tests/unit_tests/execution/";

conversionContents = ["// =============================================================================";
"// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab";
"// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>";
"//";
"//  This file is distributed under the same license as the Scilab package.";
"// =============================================================================";
"";
"// <-- CLI SHELL MODE -->";
"// <-- ENGLISH IMPOSED -->";
"// <-- LONG TIME EXECUTION -->";
"";
"// Unitary test for conversion of Matlab function: THEFUNCTIONNAME";
"";
"exec(""SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversion.sci"", -1);";
"";
"if ~m2sciTestConversion(""THEFUNCTIONNAME"") then pause;end"];

executionContents = ["// =============================================================================";
"// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab";
"// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>";
"//";
"//  This file is distributed under the same license as the Scilab package.";
"// =============================================================================";
"";
"// <-- CLI SHELL MODE -->";
"// <-- ENGLISH IMPOSED -->";
"// <-- LONG TIME EXECUTION -->";
"";
"// Unitary test for conversion of Matlab function: THEFUNCTIONNAME";
"";
"exec(""SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestExecution.sci"", -1);";
"";
"if ~m2sciTestExecution(""THEFUNCTIONNAME"") then pause;end"];

noExecutionContents = ["// =============================================================================";
"// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab";
"// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>";
"//";
"//  This file is distributed under the same license as the Scilab package.";
"// =============================================================================";
"";
"// <-- CLI SHELL MODE -->";
"// <-- ENGLISH IMPOSED -->";
"// <-- LONG TIME EXECUTION -->";
"";
"// Unitary test for conversion of Matlab function: THEFUNCTIONNAME";
"";
"exec(""SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversionNoExe.sci"", -1);";
"";
"mprintf(""%s\n"", m2sciTestConversionNoExe(""THEFUNCTIONNAME""));"];
allFunctions = listfiles(mfilesPath + "*.m");

for functionIndex = 1:size(allFunctions, "*")

    // Conversion test file
    txt = strsubst(conversionContents, "THEFUNCTIONNAME", fileparts(allFunctions(functionIndex), "fname"));
    fileName = conversionTestsPath + fileparts(allFunctions(functionIndex), "fname") + ".tst";
    mprintf("Create file %s\n", fileName);
    mputl(txt, fileName);

    // Execution test file
    txt = strsubst(executionContents, "THEFUNCTIONNAME", fileparts(allFunctions(functionIndex), "fname"));
    fileName = executionTestsPath + fileparts(allFunctions(functionIndex), "fname") + ".tst";
    mprintf("Create file %s\n", fileName);
    mputl(txt, fileName);

end

allFunctionsNoExec = listfiles(mfilesNoexecPath + "*.m");

for functionIndex = 1:size(allFunctionsNoExec, "*")
    // Conversion test file
    txt = strsubst(noExecutionContents, "THEFUNCTIONNAME", fileparts(allFunctionsNoExec(functionIndex), "fname"));
    fileName = conversionTestsPath + fileparts(allFunctionsNoExec(functionIndex), "fname") + ".tst";
    mprintf("Create file %s\n", fileName);
    mputl(txt, fileName);
end
