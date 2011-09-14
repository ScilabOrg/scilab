// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [vars_out, vars_in, fun_text] = %c_string(nameOffunction)

  vars_out = [];
  vars_in = [];
  fun_text = [];
  
  if type(nameOffunction) <> 10 then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "string", 1));
  end

  if size(nameOffunction, '*') <> 1 then
    error(999, msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), "string", 1));
  end
  
  if isdef(nameOffunction) | (whereis(nameOffunction) <> []) then
    execstr("_ptrfun = " + nameOffunction);
    r = macrovar(_ptrfun);
    vars_in = r(1);
    vars_out = r(2);
    fun_text = fun2string(_ptrfun, nameOffunction);
  else
    // never here
    error(999, msprintf(gettext("%s: Wrong value for input argument #%d: a valid function name expected.\n"), "string", 1));
  end

endfunction
