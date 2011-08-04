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
                             table, ..
                             files, ..
                             libs, ..
                             libname, ..
                             with_gateway, ..
                             ldflags, ..
                             cflags, ..
                             fflags)

  managed_cpp_ext = ['.cxx', '.cpp', '.c'];
  managed_fort_ext = ['.f90', '.f'];
  haveFortranFiles = %f;
  haveCppFiles = %f;
  
  files = matrix(files, 1, size(files, '*'));

  if typeof(table) <> 'list' then
    table = list(table);
  end
  L = length(table);

  if with_gateway then
    if L == 1 then
      files = [files , name + '.c'];
    else
       for i=1:L
        files = [files , name + string(i) + '.c'];
       end
    end
  end

  for it=1:L
    _table = table(it);
    [mt,nt] = size(_table);

    for i=1:mt
      if _table(i,3)=='cmex' | _table(i,3)=='fmex' | _table(i,3)=='Fmex' then
        MEXCFLAGS = "-Dmexfunction_=mex" + _table(i,2) + "_ -DmexFunction=mex_" + _table(i,2);
        MEXFFLAGS = "-Dmexfunction=mex" + _table(i,2);
        if _table(i,3)=='cmex' then
          filenameMex = _table(i,2) + '.c';
        else
          filenameMex = _table(i,2) + '.f';
        end
        if grep(files, filenameMex) == [] then
          files = [files , filenameMex];
        end
      end
    end
  end
  
  if ~and(isfile(files)) then
    error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
  end
  
  pause
  
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
  
  targetName = '';
  filenamesList = '';
  vcxprojDirectory = '';
  outputDirectory = '';
  additionalIncludeDir = '';
  preprocessorsDef = '';
  additionalOptionsCC = '';
  additionalDependencies = '';
  additionOptionsLD = '';
  GUID = createGUID(1);
  
  
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
        [vcxprojfullname, vcxprojGUID] = dlwCreateVcxproj(targetName, ..
                              filenamesList, vcxprojDirectory, outputDirectory, ..
                              additionalIncludeDir, preprocessorsDef, additionalOptionsCC, ..
                              additionalDependencies, additionOptionsLD, GUID);
                              
        slnfullname = dlwCreateSolution()
      else
        // not managed
      end
    end
  end
  

endfunction
//=============================================================================
