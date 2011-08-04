// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function dotnetpath = dlwGetDotNetPath()
  // we suppose that we use 4.0 as minimal framework included with VS2010
  dotnetpath = [];
  ierr = execstr("dotnetpath = winqueryreg(''HKEY_LOCAL_MACHINE'', ''SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Client'',''installPath'')", "errcatch");
  if (ierr <> 0) then
    dotnetpath = [];
  end
endfunction

