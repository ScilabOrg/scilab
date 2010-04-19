// ====================================================================
// Copyright (C) INRIA -  Serge Steer Serge.Steer@inria.fr
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ====================================================================

function datatipRemove(handle,ind)
  drawlater()
  if argn(2)==1 then //handle on a tip
    if handle.type<>"Compound" then
      warning(msprintf(_("%s: Invalid handle, waiting for a handle on a datatip"),'datatipRemove'))
      return
    end
    ind=[]
    curve_handles=datatipsGetEntities(handle.parent)
    for k=1:size(curve_handles,'*')
      ck=curve_handles(k)
      ud=datatipsGetStruct(ck);
      if typeof(ud)=='datatips' then
	ind=find(ud.tips==handle)
	if ind<>[] then
	  curve_handle=ck
	  break
	end
      end
    end
    if ind==[] then return,end
  else
    curve_handle=handle 
  end
  ud=datatipsGetStruct(curve_handle);// the curve datatips data structure
  if  typeof(ud)<>'datatips' then return,end
  if ind<=size(ud.tips,'*') then
    tips=ud.tips;
    del=tips(ind)
    tips(ind)=[]
    ud.tips=tips
    delete(del);
    datatipsSetStruct(curve_handle,ud)
  end
  drawnow()
endfunction
