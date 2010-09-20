// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Vincent LEJEUNE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================

function ret=ilib_includes_flag(path_strings)

if ( type(path_strings)<>10 )
  error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"ilib_includes_flag",1));
end

ret="";

for path=path_strings
    if ( ~isdir(fullpath(path)) )
      error(msprintf(gettext("%s: Failed to add include path : %s"),"ilib_includes_flag",path));
    end
    if (getos() == "Windows") then  
      ret=ret+' -I""'+fullpath(path)+'""';
    else
      ret=ret+" -I"+fullpath(path);
    end
end
endfunction