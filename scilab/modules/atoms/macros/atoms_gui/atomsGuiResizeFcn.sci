// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
// Copyright (C) 2013 - Samuel GOUGEON : buttons management updated
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function atomsGuiResizeFcn()
    // Load Atoms Internals lib if it's not already loaded
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    atomsfig = findobj("tag","atomsFigure");
    atomsfigPosition = atomsfig.axes_size;
    figwidth     = atomsfigPosition(1);
    figheight    = atomsfigPosition(2);

    margin       = 12;
    widgetHeight = 25;
    msgHeight    = 30;
    buttonHeight = 20;

    listboxWidth       = 200;
    listboxFrameWidth  = listboxWidth + 2*margin;
    listboxFrameHeight = figheight- 3*margin - msgHeight;
    listboxHeight      = listboxFrameHeight - 2*margin;

    descFrameWidth     = figwidth - listboxFrameWidth - 3*margin;
    descFrameHeight    = listboxFrameHeight;
    descWidth          = descFrameWidth  - 2*margin;
    descHeight         = descFrameHeight - 4*margin - buttonHeight;

    msgWidth     = figwidth -2*margin;

    // Test for objects creation
    msgText = findobj("Tag", "msgText"); // Last uicontrol to be created
    if isempty(msgText) then
        return
    end

    // Frame
    LeftFrame = findobj("Tag", "LeftFrame");
    set(LeftFrame, "Position", [margin widgetHeight+2*margin listboxFrameWidth listboxFrameHeight]);

    // Listbox
    LeftListbox = findobj("Tag", "LeftListbox");
    set(LeftListbox, "Position", [ margin margin listboxWidth listboxHeight]);

    // Frame
    DescFrame = findobj("Tag", "DescFrame");
    set(DescFrame, "Position", [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight]);

    // Frame title
    DescTitle = findobj("Tag", "DescTitle");
    set(DescTitle, "Position", [2*margin descFrameHeight-1.5*margin 200 widgetHeight]);

    // Details of a module
    Desc = findobj("Tag", "Desc");
    set(Desc, "Position", [ margin margin+buttonHeight+2*margin descWidth descHeight]);

    // Buttons
    // -------------------------------------------------------------------------
    buttons = ["autoloadCheckbox" ["install" "update" "remove" "quit" ]+"Button" ]
    nb = size(buttons,"*")
    buttonWidth = (descFrameWidth - (nb+1)*margin) / nb;
    fieldWidth = buttonWidth + margin
    for i = 1:nb
        button = findobj("Tag", buttons(i));
        set(button, "Position", [margin+(i-1)*fieldWidth margin buttonWidth widgetHeight]);
    end

    // Installed Modules: List of installed modules
    // =========================================================================

    descWidth        = descFrameWidth  - 2*margin;
    descHeight         = descFrameHeight - 3*margin;

    // Frame
    HomeFrame = findobj("Tag", "HomeFrame");
    set(HomeFrame, "Position", [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight]);

    // Frame title
    HomeTitle = findobj("Tag", "HomeTitle");
    set(HomeTitle, "Position", [2*margin descFrameHeight-1.5*margin 200 widgetHeight]);

    // Home
    HomeListbox = findobj("Tag", "HomeListbox");
    set(HomeListbox, "Position", [ margin margin descWidth descHeight]);
    updateInstalledListbox()    // updating the widths of HTML columns

    // Message Frame
    // =========================================================================

    // Frame
    msgFrame = findobj("Tag", "msgFrame");
    set(msgFrame, "Position", [margin margin msgWidth msgHeight]);

    // Text
    msgText = findobj("Tag", "msgText")
    set(msgText, "Position", [2 2 msgWidth-10 msgHeight-4]);

endfunction
