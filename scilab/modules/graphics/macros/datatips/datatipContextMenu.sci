// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function  datatipContextMenu(ax)
  curve_handles=datatipGetEntities(ax);
  [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,curve_handles)
  if curve<>[]&dmin<10 then
    ud=datatipGetStruct(curve);

    items=[_("Delete all datatips")
           _("Delete all datatips for the selected curve")
           _("Delete nearest datatip")
           _("Delete last datatip")
           _("Delete selected datatip")
           _("Select the curve tip style")
           _("Edit the curve tip display function")
           _("select the curve tip display function")];
    if ax.view=='3d' then
      items=[items; _("Redraw all datatips")];
    end
    if ud.interpolate then
      items=[items; _("Disable interpolation")];
    else
      items=[items;_("Enable interpolation")];
    end
    if ud.replace then
      items=[items;_("Multiple datatips mode")];
    else
      items=[items; _("Unique datatip mode")];
    end
  else
    items=[_("Delete all datatips");
           _("Delete selected datatip")]
  end
  sel=x_choose(items,'');
  if sel>0 then
    select items(sel)
    case _("Delete all datatips") then
      datatipRemoveAll(curve_handles);
    case _("Delete all datatips for the selected curve") then;
      datatipRemoveAll(curve);
    case _("Delete nearest datatip") then
      datatipRemoveNearest(curve);
    case _("Delete last datatip") then
      l=size(ud.tips,'*');
      if l<>0 then
        datatipRemove(curve,l)
      end
    case _("Delete selected datatip") then
      datatipDeleteSelected(curve_handles)
    case _("Edit the curve tip display function") then
      datatipSetDisplay(curve);
    case _("select the curve tip display function")  then
      datatipSelectFunction(curve);
    case _("Enable interpolation") then
      datatipSetInterp(curve,%T);
    case _("Disable interpolation") then
      datatipSetInterp(curve,%F);
    case _("Multiple datatips mode") then
      datatipSetReplaceMode(curve,%F);
    case _("Unique datatip mode") then
      datatipSetReplaceMode(curve,%t);
    case _("Redraw all datatips") then
      datatipRedraw(curve_handles);
    case _("Select the curve tip style")  then
      datatipSetStyle(curve);
    end
  end
endfunction
