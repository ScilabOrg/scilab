
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bRes = dlwIsOnlySupportedFiles(filesToCompile)
  bRes = %f;
  haveCppFiles = %f;
  haveFortranFiles = %f;
  
  if dlwIsOnlyFortranFiles(filesToCompile) | dlwIsOnlyCppFiles(filesToCompile)
    bRes = %t;
    return
  else
    filesToCompile = matrix(filesToCompile, 1, size(filesToCompile, '*'));
    haveCppFiles = %f;
    haveFortranFiles = %f;
    for i = 1:size(filesToCompile, '*')
      if ~dlwIsOnlyFortranFiles(filesToCompile(i)) & ~dlwIsOnlyCppFiles(filesToCompile(i)) then
        bRes = %f;
        return
      else
        if dlwIsOnlyCppFiles(filesToCompile(i)) then
          haveCppFiles = %t;
        end
        if dlwIsOnlyFortranFiles(filesToCompile(i)) then
          haveFortranFiles = %t;
        end
      end
    end
  end

  bRes = haveCppFiles | haveFortranFiles;

endfunction
