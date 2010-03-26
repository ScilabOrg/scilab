// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %infer_p(infertlist)

mprintf("Dims    : " + string(infertlist.dims(1)) + "x" + string(infertlist.dims(2)) + "\n");
mprintf("Type    : " + string(infertlist.type.vtype) + "\n");
mprintf("Property: " + string(infertlist.type.property) + "\n");

// If not a new infer tlist with properties
if length(infertlist) < 5 then
  mprintf("!! Old infer tlist !!\n");
  return
end

mprintf("isempty : " + string(infertlist.isempty) + "\n");
mprintf("isscalar: " + string(infertlist.isscalar) + "\n");
mprintf("isvector: " + string(infertlist.isvector) + "\n");
mprintf("ismatrix: " + string(infertlist.ismatrix) + "\n");

endfunction
