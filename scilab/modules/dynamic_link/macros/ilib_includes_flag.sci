// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC/INRIA
// Copyright (C) DIGITEO - 2009-2010 - Vincent LEJEUNE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================

function ret=ilib_includes_flag(path_strings)

if ( typeof(path_strings)~="string" )
  error(gettext("Wrong argument type : matrix of strings expected"));
end

if (getos() == "Windows") then
  ret='';
else
  ret="";
end

for path=path_strings
    if ( ~isdir(fullpath(path)) )
      error(gettext("Failed to add include path : ")+path);
    end
    if (getos() == "Windows") then  
      ret=ret+' -I""'+fullpath(path)+'""';
    else
      ret=ret+" -I"+fullpath(path);
    end
end
endfunction