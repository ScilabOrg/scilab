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

  slnfullname = [];
  files = matrix(files, 1, size(files, '*'));
  
  if ~and(isfile(files)) then
    error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
  end

  if ~dlwIsOnlySupportedFiles(files) then
    error(999, msprintf(_("%s: Wrong value for input argument #%d: file extension not managed.\n"), "ilib_gen_Make", 3));
  end
  
  if dlwIsOnlyCppFiles(files) then
    // default VS solution
    targetName = name;
    filenamesList = files;
    vcxprojDirectory = pwd();
    outputDirectory = pwd();
    additionalIncludeDir = dlwGetScilabIncludes();
    preprocessorsDef = '';
    additionalOptionsCC = cflags;
    additionOptionsLD = ldflags;
    GUID = createGUID(1);
    
    additionalDependencies = '$(SCIDIR)/bin/' + dlwGetScilabLibraries();
    if ~isempty(libs) then
      libs = matrix(libs, 1, size(libs, '*'));
      for i = 1:size(libs, '*')
        libs(i) = getrelativefilename(vcxprojDirectory, fullpath(libs(i) + ".lib"));
      end
      additionalDependencies = [additionalDependencies; libs];
    end

    if (mexcflags <> '') then
      additionalOptionsCC = additionalOptionsCC + ' ' + mexcflags;
    end

    if ~isempty(libname) then
      // bug 7481
      targetName = libname;
    end

    [vcxprojfullname, vcxprojGUID, typeGUID] = dlwCreateVcxproj(targetName, ..
              filenamesList, vcxprojDirectory, outputDirectory, ..
              additionalIncludeDir, preprocessorsDef, additionalOptionsCC, ..
              additionalDependencies, additionOptionsLD, GUID);

    dlwCreateVcxprojFilters(targetName, filenamesList, vcxprojDirectory);

    solutionName = targetName;
    solutionDirectory = vcxprojDirectory;
    slnfullname = dlwCreateSolution(solutionName, solutionDirectory, ..
                                    vcxprojfullname, vcxprojGUID, typeGUID);    
  else
    if dlwIsOnlyFortranFiles(files) then
      // fortran project or f2c
      if findmsifortcompiler() == 'ifort12' & ~dlwCheckForceF2c() then
        // Intel fortran solution

        targetName = name;
        filenamesList = files;
        vfprojDirectory = pwd();
        outputDirectory = pwd();
        additionalIncludeDir = dlwGetScilabIncludes();
        preprocessorsDef = '';
        additionalOptionsFC = fflags;
        additionOptionsLD = ldflags;
        GUID = createGUID(1);
        
        additionalDependencies = '$(SCIDIR)/bin/' + dlwGetScilabLibraries();
        if ~isempty(libs) then
          libs = matrix(libs, 1, size(libs, '*'));
          for i = 1:size(libs, '*')
            libs(i) = getrelativefilename(vfprojDirectory, fullpath(libs(i) + ".lib"));
          end
          additionalDependencies = [additionalDependencies; libs];
        end

        if (mexfflags <> '') then
          additionalOptionsFC = additionalOptionsFC + ' ' + mexfflags;
        end

        if ~isempty(libname) then
          // bug 7481
          targetName = libname;
        end
        
        [vfprojfullname, vfprojGUID, typeGUID] = dlwCreateVfproj(targetName, ..
                              filenamesList, vfprojDirectory, outputDirectory, ..
                              additionalIncludeDir, preprocessorsDef, additionalOptionsFC, ..
                              additionalDependencies, additionOptionsLD, GUID);
        
        solutionName = targetName;
        solutionDirectory = vfprojDirectory;
        slnfullname = dlwCreateSolution(solutionName, solutionDirectory, ..
                                        vfprojfullname, vfprojGUID, typeGUID);
      else
        // f2c solution
        for i=1:size(files, '*')
           if fileparts(files(i), "extension") == ".f90" then
             error(999, msprintf(_("%s: Wrong value for input argument #%d: file extension not managed by F2C.\n"), "ilib_gen_Make", 3));
             end
        end

        targetName = name;
        filenamesList = files;
        vfprojDirectory = pwd();
        outputDirectory = pwd();
        additionalIncludeDir = dlwGetScilabIncludes();
        preprocessorsDef = '';
        additionalOptionsFC = fflags;
        additionOptionsLD = ldflags;
        GUID = createGUID(1);
        
        additionalDependencies = '$(SCIDIR)/bin/' + dlwGetScilabLibraries();
        if ~isempty(libs) then
          libs = matrix(libs, 1, size(libs, '*'));
          for i = 1:size(libs, '*')
            libs(i) = getrelativefilename(vfprojDirectory, fullpath(libs(i) + ".lib"));
          end
          additionalDependencies = [additionalDependencies; libs];
        end

        if (mexfflags <> '') then
          additionalOptionsFC = additionalOptionsFC + ' ' + mexfflags;
        end

        if ~isempty(libname) then
          // bug 7481
          targetName = libname;
        end
        
        [vfprojfullname, vfprojGUID, typeGUID] = dlwCreateF2cVcxproj(targetName, ..
                              filenamesList, vfprojDirectory, outputDirectory, ..
                              additionalIncludeDir, preprocessorsDef, additionalOptionsFC, ..
                              additionalDependencies, additionOptionsLD, GUID);
        
        dlwCopyF2cRule(vfprojDirectory);
        dlwCreateF2cVcxprojFlts(targetName, filenamesList, vfprojDirectory);
        
        solutionName = targetName;
        solutionDirectory = vfprojDirectory;
        slnfullname = dlwCreateSolution(solutionName, solutionDirectory, ..
                                        vfprojfullname, vfprojGUID, typeGUID);
      end
    else
      // mixed project then we use f2c
      for i=1:size(files, '*')
        if fileparts(files(i), "extension") == ".f90" then
          error(999, msprintf(_("%s: Wrong value for input argument #%d: file extension not managed by F2C.\n"), "ilib_gen_Make", 3));
        end
      end
      
        targetName = name;
        filenamesList = files;
        vfprojDirectory = pwd();
        outputDirectory = pwd();
        additionalIncludeDir = dlwGetScilabIncludes();
        preprocessorsDef = '';
        additionalOptionsFC = fflags;
        additionOptionsLD = ldflags;
        GUID = createGUID(1);
        
        additionalDependencies = '$(SCIDIR)/bin/' + dlwGetScilabLibraries();
        if ~isempty(libs) then
          libs = matrix(libs, 1, size(libs, '*'));
          for i = 1:size(libs, '*')
            libs(i) = getrelativefilename(vfprojDirectory, fullpath(libs(i) + ".lib"));
          end
          additionalDependencies = [additionalDependencies; libs];
        end

        if (mexfflags <> '') then
          additionalOptionsFC = additionalOptionsFC + ' ' + mexfflags;
        end

        if ~isempty(libname) then
          // bug 7481
          targetName = libname;
        end
        
        [vfprojfullname, vfprojGUID, typeGUID] = dlwCreateF2cVcxproj(targetName, ..
                              filenamesList, vfprojDirectory, outputDirectory, ..
                              additionalIncludeDir, preprocessorsDef, additionalOptionsFC, ..
                              additionalDependencies, additionOptionsLD, GUID);
        
        dlwCopyF2cRule(vfprojDirectory);
        dlwCreateF2cVcxprojFlts(targetName, filenamesList, vfprojDirectory);
        
        solutionName = targetName;
        solutionDirectory = vfprojDirectory;
        slnfullname = dlwCreateSolution(solutionName, solutionDirectory, ..
                                        vfprojfullname, vfprojGUID, typeGUID);      

    end
  end

endfunction
//=============================================================================
