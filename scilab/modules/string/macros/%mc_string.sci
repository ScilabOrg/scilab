// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [vars_out, vars_in, fun_text] = %mc_string(pfunc)

  vars_out = [];
  vars_in = [];
  fun_text = [];

  if type(pfunc) <> 13 then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: a function expected.\n"), "string", 1));
  end

  r = macrovar(pfunc);
  vars_in = r(1);
  vars_out = r(2);
  fun_text = fun2string(pfunc);
  
  fun_text = strsubst(fun_text, "  ", "");
  indx = grep(fun_text, "ans(");
  if indx <> [] then
    fun_text(indx(1)) = " ";
  end

  indx = grep(fun_text, "endfunction");
  if indx <> [] then
    fun_text(indx($)) = " ";
  end

endfunction
