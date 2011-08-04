// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function [make_command, lib_name_make, lib_name, path, makename, files] = ..
             dlwGetParamsIlibCompil(lib_name, files)


  path = '';
 
  if dlwIsVc10Express() | dlwIsVc10Pro() & ~dlwCheckForceVCMakefile() then
    if dlwHaveMsBuild() & (dlwIsOnlyCppFiles(files) | dlwCheckForceF2c()) then
      make_command = 'msbuild.exe '
    else
      make_command = 'devenv.exe /Build ';
    end
    makename = lib_name + '.sln';
    lib_name = lib_name + getdynlibext();
    lib_name_make = lib_name;
  else
    lib_name = lib_name + getdynlibext();
    lib_name_make = lib_name;
    makename = dlwGetDefltMakefileName() + dlwGetMakefileExt() ;
    make_command = 'nmake /Y /nologo /f ';
  end

endfunction
//=============================================================================
