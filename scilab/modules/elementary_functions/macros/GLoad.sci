// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GLoad(name)

  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"GLoad", 1));
  end

  name1 = name;
  k = strindex(name, '.');
  if k <> [] then
    name1 = part(name1, k($)+1:length(name1));
  else
    break;
  end
  select name1
    case "sci" then exec(name),
    case "sce" then exec(name),
    case "scg" then xload(name),
    case "bin" then load(name),
    else error(msprintf(gettext("%s: Wrong value for input argument #%d: Unknown file extension, ""%s"", ""%s"", ""%s"" or ""%s"" expected"),"GLoad",1,".sci",".sce",".scg",".bin"));
  end

endfunction
