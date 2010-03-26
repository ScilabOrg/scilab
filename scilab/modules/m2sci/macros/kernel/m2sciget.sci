// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function data = m2sciget(m2sciobj, property)

select m2sciPropertyIndex(property)
case 1 // VariableType
  data = [m2sciget(m2sciobj, "IsEmpty"),..
	  m2sciget(m2sciobj, "IsScalar"),..
	  m2sciget(m2sciobj, "IsVector"),..
	  m2sciget(m2sciobj, "IsArray")];
  
case 2 // Dimensions
  data = m2sciobj.infer.dims;
  
case 3 // DataType
  data = m2sciobj.infer.type.vtype;

case 4 // IsComplex
  data = m2sciobj.infer.type.property;

case 5 // IsEmpty
  data = m2sciobj.infer.isempty;

case 6 // IsScalar
  data = m2sciobj.infer.isscalar;

case 7 // IsVector
  data = m2sciobj.infer.isvector;

case 8 // IsArray
  data = m2sciobj.infer.isarray;

else
  error(msprintf(gettext("%s: Unknown property ''%s''.\n"), "m2sciget", property));
end

endfunction

