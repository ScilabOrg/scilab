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

  function bRes = dlwHaveFortranInSln(sln)
    bRes = %f;
    if isfile(sln) then
      txt = mgetl(sln);
      bRes = grep(sln, '6989167D-11E4-40FE-8C1A-2192A86A7E90') <> [];
    end
  endfunction
 
  commandLineClean = "";
  if fileparts(makename, "extension") <> ".sln" then
    commandLineClean = 'nmake /Y /nologo /f ' + makename + dlwGetMakefileExt() + ' clean';
  else
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
  
    if dlwHaveFortranInSln(makename) | ~dlwHaveMsBuild() then
      commandLineClean = "devenv.exe /clean " + solutionConfig + " """ + makename + """";
    else
      commandLineClean = "msbuild.exe " + " " + makename + " /t:clean /p:Platform=" + platformConfig + ..
                          " /p:Configuration=" + solutionConfig;
    end
  end
  cmdl = commandLineClean;
  
endfunction
//=============================================================================
