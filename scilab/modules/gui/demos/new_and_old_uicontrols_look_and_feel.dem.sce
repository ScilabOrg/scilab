// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

c = get(0);c.usedeprecatedskin = "off";
f = figure("dockable", "off" ,"infobar_visible", "off", "toolbar_visible", "off", "toolbar", "none", "menubar_visible", "off", "menubar", "none", "default_axes", "off", "visible", "off", "layout", "border");
f.figure_id = 100001;

fr = uicontrol("style", "frame");

fr1 = uicontrol(fr, "style", "frame", "position", [10 10 220 390], "border", createBorder("etched"));
editbox = uicontrol(fr1, "style", "edit", "string", "text", "position", [10 350 200 30]);
label = uicontrol(fr1, "style", "text", "string", "label", "position", [10 310 200 30]);
button = uicontrol(fr1, "style", "pushbutton", "string", "bouton", "position", [10 270 200 30]);
checkbox = uicontrol(fr1, "style", "checkbox", "string", "case a cocher", "position", [10 230 200 30]);
radio = uicontrol(fr1, "style", "radiobutton", "string", "case a cocher", "position", [10 190 200 30]);
slider = uicontrol(fr1, "style", "slider", "position", [10 150 200 30], "min", 0, "max", 100);
frame = uicontrol(fr1, "style", "frame", "position", [10 110 200 30]);
listbox = uicontrol(fr1, "style", "listbox", "string", "list1|list2|list3|list4|list5", "position", [10 50 200 50]);
combo = uicontrol(fr1, "style", "popupmenu", "string", "list1|list2", "position", [10 10 200 30]);

c = get(0);c.usedeprecatedskin = "on";
fr2 = uicontrol(fr, "style", "frame", "position", [240 10 220 390]);
editbox2 = uicontrol(fr2, "style", "edit", "string", "text", "position", [10 350 200 30]);
label2 = uicontrol(fr2, "style", "text", "string", "label", "position", [10 310 200 30]);
button2 = uicontrol(fr2, "style", "pushbutton", "string", "bouton", "position", [10 270 200 30]);
checkbox2 = uicontrol(fr2, "style", "checkbox", "string", "case a cocher", "position", [10 230 200 30]);
radio2 = uicontrol(fr2, "style", "radiobutton", "string", "case a cocher", "position", [10 190 200 30]);
slider2 = uicontrol(fr2, "style", "slider", "position", [10 150 200 30], "min", 0, "max", 100);
frame2 = uicontrol(fr2, "style", "frame", "position", [10 110 200 30]);
listbox2 = uicontrol(fr2, "style", "listbox", "string", "list1|list2|list3|list4|list5", "position", [10 50 200 50]);
combo2 = uicontrol(fr2, "style", "popupmenu", "string", "list1|list2", "position", [10 10 200 30]);


f.axes_size = [470, 410];
f.visible = "on";
