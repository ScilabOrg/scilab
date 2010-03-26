// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function m2sciobj = m2sciset(m2sciobj, property, value)

// Ignore if in a particular macro
[l,mac] = where()
if or(mac == "m2sci_info") then
  return
end

// Convert old infer tlist to new format
%infer = m2sciobj.infer;
if length(%infer) < 5 then
  m2scidebug("!! Old infer structure !!");
  %infer.isempty = Unknown;
  %infer.isscalar = Unknown;
  %infer.isvector = Unknown;
  %infer.isarray = Unknown;
end
m2sciobj.infer = %infer;

select m2sciPropertyIndex(property)
case 1 // VariableType
  if size(value, "*") == 4 then
    if sum(bool2s(value==%T)) > 1 then
      // Only when type can be set
      error("Wrong value!");
    else
      m2sciobj = m2sciset(m2sciobj, "IsEmpty", value(1));
      m2sciobj = m2sciset(m2sciobj, "IsScalar", value(2));
      m2sciobj = m2sciset(m2sciobj, "IsVector", value(3));
      m2sciobj = m2sciset(m2sciobj, "IsArray", value(4));
    end
  else
    error("Wrong size!");
  end
  
case 2 // Dimensions
  matDims = [];
  for k = 1:lstsize(value)
    matDims = [matDims , value(k)];
  end
  if prod(matDims) == 0 then
    m2scidebug("Set empty according to dims");
    m2sciobj = m2sciset(m2sciobj, "IsEmpty", %T);
  end
  if prod(matDims) == 1 then
    m2scidebug("Set scalar according to dims");
    m2sciobj = m2sciset(m2sciobj, "IsScalar", %T);
  end
  if prod(matDims) <> 1 & or(matDims==1) then
    m2scidebug("Set vector according to dims");
    m2sciobj = m2sciset(m2sciobj, "IsVector", %T);
  end
  if size(find(matDims>1), "*") >= 2 then
    m2scidebug("Set array according to dims");
    m2sciobj = m2sciset(m2sciobj, "IsArray", %T);
  end
  m2sciobj.infer.dims = value;
  
  
case 3 // DataType
  // TODO: change this !!
  m2sciobj.infer.type = Type(value, m2sciget(m2sciobj, "IsComplex"));

case 4 // IsComplex
  // TODO: change this !!
  m2sciobj.infer.type = Type(m2sciget(m2sciobj, "DataType"), value);

case 5 // IsEmpty
  %infer = m2sciobj.infer;
  if value==%T then
    checkChanges(m2sciobj, [value, %F, %F, %F])
    %infer.isscalar = %F;
    %infer.isvector = %F;
    %infer.isarray = %F;
  end
  %infer.isempty = value;
  m2sciobj.infer = %infer;
  
case 6 // IsScalar
  %infer = m2sciobj.infer;
  if value==%T then
    checkChanges(m2sciobj, [%F, value, %F, %F])
    %infer.isempty = %F;
    %infer.isvector = %F;
    %infer.isarray = %F;
  end
  %infer.isscalar = value;
  m2sciobj.infer = %infer;
  
case 7 // IsVector
  %infer = m2sciobj.infer;
  if value==%T then
    checkChanges(m2sciobj, [%F, %F, value, %F])
    %infer.isempty = %F;
    %infer.isscalar = %F;
    %infer.isarray = %F;
  end
  %infer.isvector = value;
  m2sciobj.infer = %infer;
 
case 8 // IsArray
  %infer = m2sciobj.infer;
  if value==%T then
    checkChanges(m2sciobj, [%F, %F, %F, value])
    %infer.isempty = %F;
    %infer.isscalar = %F;
    %infer.isvector = %F;
  end
  %infer.isarray = value;
  m2sciobj.infer = %infer;
  
case 9 // AllProperties
  // Convert old infer tlist to new format
  %infer = value.infer;
  if length(%infer) < 5 then
    m2scidebug("!! Old infer structure !!");
    %infer.isempty = Unknown;
    %infer.isscalar = Unknown;
    %infer.isvector = Unknown;
    %infer.isarray = Unknown;
  end
  value.infer = %infer;
  managedProperties = m2sciProperties();
  for k = 1:(size(managedProperties, "*") - 1)
    property = managedProperties(k);
    m2sciobj = m2sciset(m2sciobj, property, m2sciget(value, property));
  end
  
else
  error(msprintf(gettext("%s: Unknown property ''%s''.\n"), "m2sciset", property));
end

endfunction

function checkChanges(m2sciobj, value)

%infer = m2sciobj.infer;

// If bool is %T then have to check old value
ise = %infer.isempty==%T;
iss = %infer.isscalar==%T;
isv = %infer.isvector==%T;
ism = %infer.isarray==%T;

index = find(value);
props = ["empty","scalar","vector","array"];

if or(value) & or([ise iss isv ism]) then
  
  if index <> find([ise iss isv ism]) then
    m2scidebug("VariableType changed from ''" + ...
	props(find([ise iss isv ism])) + "'' to ''" + ...
	props(index) + "''");
  else
    m2scidebug("VariableType set to ''" + props(index) + "'' (Was already set...)");
  end

else
  m2scidebug("VariableType set to ''" + props(index) + "''");
end
endfunction

function m2scidebug(msg)
return
mprintf("At line " + string(nblines) + ": ");
mprintf(strsubst(expression2code(m2sciobj),"%", "%%"));
mprintf(": ");
mprintf(msg);
mprintf("\n");
endfunction
