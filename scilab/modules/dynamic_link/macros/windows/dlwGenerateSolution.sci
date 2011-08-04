// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function slnfullname = dlwGenerateSolution(name, ..
                             files, ..
                             libs, ..
                             libname, ..
                             ldflags, ..
                             cflags, ..
                             fflags, ..
                             mexcflags, ..
                             mexfflags)


  managed_cpp_ext = ['.cxx', '.cpp', '.c'];
  managed_fort_ext = ['.f90', '.f'];
  haveFortranFiles = %f;
  haveCppFiles = %f;
  
  files = matrix(files, 1, size(files, '*'));
  
  if ~and(isfile(files)) then
    error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
  end
  
  for i = 1:size(files,'*')
    [path_f, file_f, ext_f] = fileparts(files(i));
    if ~or(ext_f == managed_cpp_ext) & ~or(ext_f == managed_fort_ext) then
      error(999, msprintf(_("%s: Wrong value for input argument #%d: file extension not managed %s.\n"), "ilib_gen_Make", 3, ext_f));
    else
      if or(ext_f == managed_cpp_ext) then
        haveCppFiles = %t;
      end
      if or(ext_f == managed_cpp_ext) then
        haveFortranFiles = %f;
      end
    end
  end
  
  
  if haveFortranFiles & haveCppFiles then
    // f2c solution
  else
    if haveFortranFiles & ~haveCppFiles then
      if findmsifortcompiler() == 'ifort12' then
        // Intel fortran solution
      else
        // f2c solution
      end
    else
      if ~haveFortranFiles & haveCppFiles then
        // default VS solution

        targetName = name;
        filenamesList = files;
        vcxprojDirectory = pwd();
        outputDirectory = pwd();
        additionalIncludeDir = dlwGetScilabIncludes();
        preprocessorsDef = '';
        additionalOptionsCC = '';
        additionalDependencies = dlwGetScilabLibraries();
        additionOptionsLD = '';
        GUID = createGUID(1);
        
        [vcxprojfullname, vcxprojGUID, typeGUID] = dlwCreateVcxproj(targetName, ..
                              filenamesList, vcxprojDirectory, outputDirectory, ..
                              additionalIncludeDir, preprocessorsDef, additionalOptionsCC, ..
                              additionalDependencies, additionOptionsLD, GUID);
                              
        dlwCreateVcxprojFilters(targetName, filenamesList, vcxprojDirectory);

        slnfullname = dlwCreateSolution(solutionName, solutionDirectry, vcxprojfullname, vcxprojGUID, typeGUID);
      else
        // not managed
      end
    end
  end

endfunction
//=============================================================================
