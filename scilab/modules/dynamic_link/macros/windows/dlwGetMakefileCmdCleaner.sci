// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function cmdl = dlwGetMakefileCmdCleaner(makename)

  commandLineClean = "";
  
  if dlwIsVc10Express() | dlwIsVc10Pro() & ~dlwCheckForceVCMakefile() then
    solutionConfig = "";
    platformConfig = "";
    
    if dlwCheckDebugMode() then
      solutionConfig = "Debug";
    else
      solutionConfig = "Release";
    end
    
    if win64() then
      platformConfig = "x64";
    else
      platformConfig = "win32";
    end
    
    solutionConfig = solutionConfig + "|" + platformConfig;
    solutionConfig = """" + solutionConfig + """";
    
    if dlwIsVc10Express() then
    end
    commandLineClean = "devenv.exe /clean " + solutionConfig + " """ + makename + """";
  else
    commandLineClean = 'nmake /Y /nologo /f ' + makename + dlwGetMakefileExt() + ' clean';
  end
  
  cmdl = commandLineClean;
  
endfunction
//=============================================================================
