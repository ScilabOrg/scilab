// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [result,err] = get_param(list_name,param_name,param_default)
[nargout,nargin] = argn();
if ~isdef('param_default','local') then
  param_default = [];
end

result = param_default;

if typeof(list_name)=='plist' then
  if is_param(list_name,param_name) then
    result = list_name(param_name);
    if nargout==2 then 
      err = %F; 
    end
  else
    if nargout==2 then 
      err = %T; 
    else
      warning(sprintf(gettext("%s: parameter %s not defined"),"get_param",param_name));
    end
  end
else
  if nargout==2 then 
    err = %T; 
  else
    error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "get_param", 1, "plist"));
  end
end
endfunction
