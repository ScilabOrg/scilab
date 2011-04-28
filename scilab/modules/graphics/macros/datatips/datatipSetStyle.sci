// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetStyle(curve_handle,t,boxed)
//changes the tips interpolation modes for a given curve
// curve_handle : a handle on a polyline
// t            : 1 "square mark" or 2 "Directional arrow"
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),...
                   "datatipSetType",1))
  end
  if argn(2)==2 then
    boxed=%t
  end
  
  if type(curve_handle)<>9|or(curve_handle.type<>"Polyline") then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                   "datatipSetType",1,"Polyline"))
  end

  if argn(2)==1 then
    items=[_("Square mark, boxed label")
           _("Square mark, simple label")
           _("Directional arrow, boxed label")
           _("Directional arrow, simple label")];
    sel=x_choose(items,_("Select tip style"))
    if sel==0 then return,end
    style=[floor((sel+1)/2) modulo(sel,2)]
  else
    if type(t)<>1|size(t,'*')<>1 then 
      error(msprintf(_("%s: Wrong type for input argument #%d: A scalar expected.\n"),...
                   "datatipSetType",2))
    end
    if and(t<>[1 2]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),...
                   "datatipSetType",2,"1,2"))
    end
    if type(boxed)<>4 |size(t,'*')<>1 then 
      error(msprintf(_("%s: Wrong type for input argument #%d: A booelan scalar expected.\n"),...
                   "datatipSetType",3))
    end
    style=[t bool2s(boxed)]
  end

  ud=datatipGetStruct(curve_handle)
  if typeof(ud)<>'datatips' then;
    datatipInitStruct(curve_handle)
    ud=datatipGetStruct(curve_handle)
  end
  if or(ud.style<>style) then
    drawlater()
    for i=1:size(ud.tips,'*')
      datatipSetTipStyle(ud.tips(i),style)
    end
    drawnow()
    ud.style=style
    datatipSetStruct(curve_handle,ud)
  end
  
endfunction
