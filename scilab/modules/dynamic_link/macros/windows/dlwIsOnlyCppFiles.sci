
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bRes = dlwIsOnlyCppFiles(filesToCompile)
  bRes = %t;

  filesToCompile = matrix(filesToCompile, 1, size(filesToCompile, '*'));
  
  for i = 1:size(filesToCompile,'*')
    [path_f, file_f, ext_f] = fileparts(filesToCompile(i));
    if ~or( ext_f == dlwGetCppExtManaged() ) then
      bRes = %f;
      return
    end
  end
endfunction
//=============================================================================
