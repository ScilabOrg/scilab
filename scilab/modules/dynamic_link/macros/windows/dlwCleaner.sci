// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function dlwCleaner(makename)
  if fileinfo(makename) <> [] then
    make_command = dlwGetMakefileCmdCleaner(makename);

    if fileparts(makename,"extension") == ".sln" then
      unix_s(make_command);
      mdelete(makename);
      path_solution = fileparts(makename,'path');
      vcxproj = strsubst(makename, ".sln", ".vcxproj");
      vfproj = strsubst(makename, ".sln", ".vfproj");
      mdelete(vcxproj);
      mdelete(vfproj);
      mdelete(vcxproj + ".user");
      mdelete(vcxproj + ".filters");
      mdelete(strsubst(makename, ".sln", ".def"));
      mdelete(strsubst(makename, ".sln", ".lib"));
      mdelete(strsubst(makename, ".sln", ".exp"));
      mdelete(strsubst(makename, ".sln", ".u2d"));
      mdelete(strsubst(makename, ".sln", ".suo"));
      mdelete(strsubst(makename, ".sln", ".sdf"));
      mdelete(path_solution + "f2c.props");
      mdelete(path_solution + "f2c.targets");
      mdelete(path_solution + "f2c.xml");
      rmdir(ipch, 's');
    else // makefile
      unix_s(make_command);
      mdelete(makename + dlwGetMakefileExt());
    end

    if dlwCheckDebugMode() then
      solutionConfig = "Debug";
    else
      solutionConfig = "Release";
    end
    rmdir(solutionConfig, 's');
  end
endfunction
//=============================================================================
