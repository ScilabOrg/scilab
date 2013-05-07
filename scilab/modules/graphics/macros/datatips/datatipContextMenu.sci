// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function  datatipContextMenu(ax)
    //datatip utility function
    f=gcf();
    curve_handles=datatipGetEntities(ax);
    [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,curve_handles)
    sel_items = 0;
    
    if curve<>[]&dmin<10 then
        ud=datatipGetStruct(curve);
        hMenu = uicontextmenu();
        items=[_("Delete all datatips")
        _("Delete all datatips for the selected curve")
        _("Delete nearest datatip")
        _("Delete last datatip")
        _("Delete selected datatip")
        _("Open curve tip style editor")
        _("Redraw all datatips")];

        hItem1 = uimenu("Label", items(1), "Parent", hMenu, "Callback", "sel_items=1");
        hItem2 = uimenu("Label", items(2), 'Parent', hMenu, 'Callback', "sel_items=2");
        hItem3 = uimenu("Label", items(3), "Parent", hMenu, "Callback", "sel_items=3");
        hItem4 = uimenu("Label", items(4), "Parent", hMenu, "Callback", "sel_items=4");
        hItem5 = uimenu("Label", items(5), "Parent", hMenu, "Callback", "sel_items=5");
        hItem6 = uimenu("Label", items(6), "Parent", hMenu, "Callback", "sel_items=6");
        hItem7 = uimenu("Label", items(7), "Parent", hMenu, "Callback", "sel_items=7");
    else
        hMenu = uicontextmenu();
        items=[_("Delete all datatips");
        _("Delete selected datatip")
        _("Redraw all datatips")];
        hItem1 = uimenu("Label", items(1), "Parent", hMenu, "Callback", "sel_items=1");
        hItem5 = uimenu("Label", items(2), "Parent", hMenu, "Callback", "sel_items=5");
        hItem7 = uimenu("Label", items(3), "Parent", hMenu, "Callback", "sel_items=7");
    end

    [%v0,%v1,%v2,%v3,%v4] = xclick()
    if %v4 <> "void" then 
        execstr(%v4);
        if sel_items == 1 then
            datatipRemoveAll(curve_handles);
        elseif sel_items == 2 then
            datatipRemoveAll(curve);
        elseif sel_items == 3 then
            datatipRemoveNearest(curve,pt);
        elseif sel_items == 4 then
            l=size(ud.tips.children,'*');
            if l<>0 then
                datatipRemove(curve,1)
            end
        elseif sel_items == 5 then
            datatipDeleteSelected(curve_handles);
        elseif sel_items == 6 then
            datatipsGUI(curve)
            scf(f);
        elseif sel_items == 7 then
            datatipRedraw(curve_handles);
        end
    end
endfunction
