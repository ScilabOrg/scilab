// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function dlwGenerateCleaner(fd, makename)
  make_command = dlwGetMakefileCmdCleaner(makename);
  if dlwIsVc10Express() | dlwIsVc10Pro() & ~dlwCheckForceVCMakefile() then
    mfprintf(fd,"if fileinfo(''%s'') <> [] then\n", makename);
    mfprintf(fd,"  unix_s(''%s'');\n", make_command);
    mfprintf(fd,"  mdelete(''%s'');\n", makename);
    vcxproj = strsubst(makename, ".sln", ".vcxproj");
    vfproj = strsubst(makename, ".sln", ".vfproj");
    mfprintf(fd,"  mdelete(''%s'');\n", vfproj);
    mfprintf(fd,"  mdelete(''%s'');\n", vcxproj);
    mfprintf(fd,"  mdelete(''%s'');\n", vcxproj + ".user");
    mfprintf(fd,"  mdelete(''%s'');\n", vcxproj +  ".filters");    
    mfprintf(fd,"  mdelete(''%s'');\n", strsubst(makename, ".sln", ".def"));
    mfprintf(fd,"  mdelete(''%s'');\n", strsubst(makename, ".sln", ".lib"));
    mfprintf(fd,"  mdelete(''%s'');\n", strsubst(makename, ".sln", ".exp"));
    mfprintf(fd,"  mdelete(''%s'');\n", strsubst(makename, ".sln", ".u2d"));
    if dlwCheckDebugMode() then
      solutionConfig = "Debug";
    else
      solutionConfig = "Release";
    end
    mfprintf(fd,"  rmdir(''%s'', ''s'');\n", solutionConfig);
  else
    mfprintf(fd,"if fileinfo(''%s%s'') <> [] then\n", makename, dlwGetMakefileExt());
    mfprintf(fd,"  unix_s(''%s'');\n", make_command);
    mfprintf(fd,"  mdelete(''%s%s'');\n", makename, dlwGetMakefileExt());
  end
  mfprintf(fd,"end\n");
  mfprintf(fd,"// ------------------------------------------------------\n");
endfunction
//=============================================================================
