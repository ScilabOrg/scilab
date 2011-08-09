// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipRemoveAll(curve_handles)
//remove all the datatips for the given curves
  if argn(2)<1 then
    ax=gca();
    curve_handles=datatipGetEntities(ax)
  else
    ax=curve_handles(1).parent;
    while ax.type<>"Axes" then ax=ax.parent,end
  end

  fig=ax.parent
  id=fig.immediate_drawing;
  fig.immediate_drawing="off"

  for k=1:size(curve_handles,'*')
    ck=curve_handles(k);
    ud=datatipGetStruct(ck)// the curve datatips data structure
    if typeof(ud)=='datatips' then
      for i=1:size(ud.tips,'*'), delete(ud.tips(i));end
      ud.tips=[]
      datatipSetStruct(ck,ud)
    end
  end
  fig.immediate_drawing=id
endfunction
