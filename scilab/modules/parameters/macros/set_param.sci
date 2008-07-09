
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ga_list,err] = set_param(list_name,param_name,param_value)
[nargout,nargin] = argn();
ga_list = list_name;
if typeof(list_name)=='plist' then
  if nargout==2 then err = %F; end
  if is_param(ga_list,param_name) then
    ga_list(param_name) = param_value;
  else
    err = %T;
    warning(sprintf(gettext("%s: parameter doesn''t exist"),"set_param"));
  end
else
  if nargout==2 then 
    err = %T;
  else
    warning(sprintf(gettext("%s: not a plist"),"set_param"));
  end
end
endfunction

