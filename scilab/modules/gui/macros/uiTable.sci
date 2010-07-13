// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Han Dong
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function h=uiTable(varargin)

Rhs = length(varargin);

//creates a preliminary uitable
h = uicontrol("style", "uitable", "position", [0 0 300 300]);

//counter is used to track user input arguments
counter=0;

if Rhs > 0 then
  //sets the counter to 1 if the user does not input the correct arguments
  for i=1:2:Rhs
    s = varargin(i);
    if s == 'colnames' | s == 'rownames' | s == 'tabledata' | s == 'position' then
    else
      counter=1;
    end
  end
  
  //if counter is equal to 1, gives an error output.
  if counter==1 then
    error(msprintf(gettext("%s: Unidentified input argument(s). Only colnames, rownames, tabledata, position are accepted.\n"), "uiTable"));
  else
    //otherwise, calls the respective functions for setting table colnames, rownames, tabledata, position
    for i=1:2:Rhs
      propertyName = varargin(i);
      propertyValue = varargin(i+1);
      set(h, propertyName, propertyValue);
    end
  end
end
endfunction
//-----------------------------------------------------------------------------
