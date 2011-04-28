// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function  datatipContextMenu1(ax)
  curve_handles=datatipGetEntities(ax);
  [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,curve_handles)
  if curve<>[]&dmin<10 then
    ud=datatipGetStruct(curve);
    submenus=list([_("Delete all datatips"), "datatipRemoveAll(gcbo.user_data);"],..
                  [_("Delete all datatips for the selected curve")," datatipRemoveAll(curve);"],...
                  [_("Delete nearest datatip"),"datatipRemoveNearest(curve);"],...
                  [_("Delete last datatip"),""],...
                  [_("Delete selected datatip"),"datatipDeleteSelected(curve_handles)"],...
                  [_("Edit the curve tip display function"),"datatipSetDisplay(curve);"],...
                  [_("select the curve tip display function"),"datatipSelectFunction(curve);"])
    if ax.view=='3d' then
      submenus($+1)=[_("Redraw all datatips"),"datatipRedraw(curve_handles)"]
    end
    if ud.interpolate then
      submenus($+1)=[_("Disable interpolation"),"datatipSetInterp(curve,%F)"];
    else
      submenus($+1)=[_("Enable interpolation"),"datatipSetInterp(curve,%T)"];
    end
    if ud.replace then
      submenus($+1)=[_("Multiple datatips mode"),"datatipSetReplaceMode(curve,%F);"];
    else
      submenus($+1)=[_("Unique datatips mode"),"datatipSetReplaceMode(curve,%t);"];
    end
  else
    submenus=list([_("Delete all datatips"),"datatipRemoveAll(curve_handles);"],..
                  [_("Delete selected datatip"),"datatipDeleteSelected(curve_handles)"])
  end
  h = datatipCreatePopupMenu(submenus)
endfunction
