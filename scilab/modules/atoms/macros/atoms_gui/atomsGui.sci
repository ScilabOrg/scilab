// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - Samuel GOUGEON : bug fixed
// Copyright (C) 2012 - DIGITEO - Allan CORNET
// Copyright (C) 2013 - Samuel GOUGEON : buttons: autoload checkbox + quit added.
//                                                lighter background
//                                       installed list: becomes updatable
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function atomsGui()

    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Test connection
    allModules = [];
    errStatus  = execstr("allModules = atomsDESCRIPTIONget();", "errcatch");

    if errStatus<>0 | size(allModules, "*") == 0 then
        if size(atomsRepositoryList(),"*") > 0 then
            messagebox(gettext("No ATOMS module is available. Please, check your Internet connection or make sure that your OS is compatible with ATOMS."), gettext("ATOMS error"), "error");
        else
            messagebox(gettext("No ATOMS module is available: your repository list is empty."), gettext("ATOMS error"), "error");
        end
        return
    end

    // Parameters
    // =========================================================================

    // Figure width & height
    figwidth     = 800;
    figheight    = 500;

    // Margin
    margin       = 12;
    widgetHeight   = 25;

    // Message Frame
    msgWidth     = figwidth -2*margin;
    msgHeight    = 30;

    // Button
    buttonHeight   = 20;

    // Font Size
    defaultFontSize  = 12;

    // Close the window if it already exists
    oldFig = findobj("tag", "atomsFigure");
    if ~isempty(oldFig) then
        delete(oldFig);
    end

    // Create the main window
    // =========================================================================

    atomsfig = figure( ..
    "figure_name", gettext("ATOMS"), ..
    "position"   , [0 0 figwidth figheight],..
    "background" , -2,..
    "UserData"   , allModules, ..
    "ResizeFcn", "atomsGuiResizeFcn", ..
    "tag"    , "atomsFigure");

    // Remove Scilab graphics menus & toolbar
    // =========================================================================

    delmenu(atomsfig.figure_id, gettext("&File"));
    delmenu(atomsfig.figure_id, gettext("&Tools"));
    delmenu(atomsfig.figure_id, gettext("&Edit"));
    delmenu(atomsfig.figure_id, gettext("&?"));
    toolbar(atomsfig.figure_id, "off");

    // Add ATOMS Menu
    // =========================================================================

    // Menu File
    h = uimenu( ..
    "parent"   , atomsfig, ..
    "label"  , gettext("File"));

    // Menu File:Installed Modules
    uimenu( ..
    "parent"   , h, ..
    "label"  , gettext("Installed modules"), ..
    "callback" , "cbAtomsGui", ..
    "tag"    , "homeMenu");

    // Menu File:Update List of Packages
    uimenu( ..
    "parent"   , h, ..
    "label"  , gettext("Update List of Packages"), ..
    "callback" , "xinfo(gettext(''Updating the list of packages. Please wait... until Done.''));" + ..
    "atomsSystemUpdate();" + ..
    "xinfo(gettext(''Update done.''));" , ..
    "tag"    , "updatePackages");

    // Menu File:Close
    uimenu( ..
    "parent"   , h, ..
    "label"  , gettext("Close"), ..
    "callback" , "cbAtomsGui", ..
    "tag"    , "closeMenu");

    // Menu ?
    h = uimenu( ..
    "parent"   , atomsfig, ..
    "label"  , gettext("?"));

    // Menu ?:Atoms Help...
    uimenu( ..
    "parent"   , h, ..
    "label"  , gettext("Atoms Help..."), ..
    "callback" , "cbAtomsGui", ..
    "tag"    , "helpMenu");

    // Build the module list (listbox on the left)
    // =========================================================================
    LeftElements = atomsGetLeftListboxElts("filter:main");

    // Set the figure size ... after all delmenu(s)
    // =========================================================================
    atomsfig.axes_size = [figwidth figheight];

    // List of modules
    // =========================================================================

    listboxWidth        = 200;
    listboxFrameWidth     = listboxWidth + 2*margin;

    listboxFrameHeight    = figheight- 3*margin - msgHeight;
    listboxHeight       = listboxFrameHeight - 2*margin;

    // Figure name
    atomsfig("figure_name")   = LeftElements("title")+" - ATOMS";

    // Frame
    LeftFrame         = uicontrol( ..
    "Parent"        , atomsfig,..
    "Style"         , "frame",..
    "Relief"        , "solid",..
    "Position"      , [margin widgetHeight+2*margin listboxFrameWidth listboxFrameHeight],..
    "Background"      , [1 1 1],..
    "UserData"      , "filter:main",..
    "Tag"         , "LeftFrame");

    // Listbox
    LeftListbox         = uicontrol( ..
    "Parent"        , LeftFrame,..
    "Style"         , "listbox",..
    "Position"      , [ margin margin listboxWidth listboxHeight],..
    "Background"      , [1 1 1],..
    "FontSize"      , defaultFontSize,..
    "String"        , LeftElements("items_str"),..
    "UserData"      , LeftElements("items_mat"),..
    "Callback"      , "cbAtomsGui", ..
    "Min"         , 1, ..
    "Max"         , 1, ..
    "Tag"         , "LeftListbox")

    // Description of a module
    // =========================================================================

    descFrameWidth       = figwidth - listboxFrameWidth - 3*margin;
    descFrameHeight      = listboxFrameHeight;

    descWidth        = descFrameWidth  - 2*margin;
    descHeight         = descFrameHeight - 4*margin - buttonHeight;

    // Frame
    DescFrame        = uicontrol( ..
    "Parent"       , atomsfig,..
    "Style"        , "frame",..
    "Relief"       , "solid",..
    "Background"     , [1 1 1],..
    "Position"       , [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight],..
    "Tag"        , "DescFrame", ..
    "Visible"      , "off");

    // Frame title
    DescTitle        = uicontrol( ..
    "Parent"       , DescFrame,..
    "Style"        , "text",..
    "Position"       , [2*margin descFrameHeight-1.5*margin 200 widgetHeight],..
    "HorizontalAlignment", "center",..
    "VerticalAlignment"  , "middle",..
    "String"       , gettext("List of installed modules"), ..
    "FontWeight"     , "bold",..
    "FontSize"       , 12,..
    "Background"     , [1 1 1],..
    "Tag"        , "DescTitle");

    // Details of a module
    Desc           = uicontrol( ..
    "Parent"       , DescFrame,..
    "VerticalAlignment"  , "top",..
    "Style"        , "text",..
    "Position"       , [ margin margin+buttonHeight+2*margin descWidth descHeight],..
    "Background"     , [1 1 1],..
    "FontSize"       , defaultFontSize,..
    "String"       , "",..
    "Tag"        , "Desc");

    // Buttons
    // -------------------------------------------------------------------------

    buttonWidth = (descFrameWidth - 6*margin) / 5;
    fieldWidth = buttonWidth + margin
    i = -1

    // "Autoload"" Checkbox
    i = i+1
    autoloadCheckbox = uicontrol( ..
    "Parent"     , DescFrame,..
    "Style"      , "checkbox",..
    "Position"   , [ margin+i*fieldWidth margin buttonWidth widgetHeight ],..
    "String"     , gettext("Load at startup"),..
    "background" , [ 1 1 1 ], ..
    "Callback"   , "cbAtomsGui", ..
    "Min"        , 0, ..
    "Max"        , 1, ..
    "Tag"        , "autoloadCheckbox")

    // "Install" Button
    i = i+1
    installButton = uicontrol( ..
    "Parent"    , DescFrame,..
    "Style"     , "pushbutton",..
    "Position"  , [ margin+i*fieldWidth margin buttonWidth widgetHeight ],..
    "String"    , gettext("Install"),..
    "background" , [ 0.9 0.9 0.9 ], ..
    "Callback"  , "cbAtomsGui", ..
    "Enable"    , "off", ..
    "Tag"       , "installButton");

    // "Update" Button
    i = i+1
    updateButton  = uicontrol( ..
    "Parent"    , DescFrame,..
    "Style"     , "pushbutton",..
    "Position"  , [ margin+i*fieldWidth margin buttonWidth widgetHeight ],..
    "String"    , gettext("Update"),..
    "background" , [ 0.9 0.9 0.9 ], ..
    "Callback"  , "cbAtomsGui", ..
    "Enable"    , "off", ..
    "Tag"       , "updateButton");

    // "Remove" Button
    i = i+1
    removeButton  = uicontrol( ..
    "Parent"    , DescFrame,..
    "Style"     , "pushbutton",..
    "Position"  , [ margin+i*fieldWidth margin buttonWidth widgetHeight ],..
    "String"    , gettext("Remove"),..
    "background" , [ 0.9 0.9 0.9 ], ..
    "Callback"  , "cbAtomsGui", ..
    "Enable"    , "off",..
    "Tag"       , "removeButton");

    // "Quit" Button
    i = i+1
    updateButton  = uicontrol( ..
    "Parent"    , DescFrame,..
    "Style"     , "pushbutton",..
    "Position"  , [margin+i*fieldWidth margin buttonWidth widgetHeight],..
    "String"    , gettext("Quit"),..
    "background" , [ 0.9 0.9 0.9 ], ..
    "Callback"  , "cbAtomsGui", ..
    "Tag"       , "quitButton");

    // Installed Modules: List of installed modules
    // =========================================================================

    descWidth        = descFrameWidth  - 2*margin;
    descHeight       = descFrameHeight - 3*margin;

    // Frame
    HomeFrame        = uicontrol( ..
    "Parent"       , atomsfig,..
    "Style"        , "frame",..
    "Relief"       , "solid",..
    "Background"   , [1 1 1],..
    "Position"     , [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight],..
    "Tag"          , "HomeFrame");

    // Frame title
    HomeTitle        = uicontrol( ..
    "Parent"       , HomeFrame,..
    "Style"        , "text",..
    "Position"     , [2*margin descFrameHeight-1.5*margin 200 widgetHeight],..
    "HorizontalAlignment", "center",..
    "VerticalAlignment"  , "middle",..
    "String"       , gettext("List of installed modules"), ..
    "FontWeight"   , "bold",..
    "FontSize"     , 12,..
    "Background"   , [1 1 1],..
    "Tag"          , "HomeTitle");

    // Home
    HomeListbox         = uicontrol( ..
    "Parent"        , HomeFrame,..
    "Style"         , "listbox",..
    "Position"      , [ margin margin descWidth descHeight],..
    "Background"      , [1 1 1],..
    "FontSize"      , defaultFontSize,..
    "Callback"      , "cbAtomsGui", ..
    "Min"         , 1, ..
    "Max"         , 1, ..
    "Tag"         , "HomeListbox");

    // Build the list of installed modules only AFTER the box has been defined:
    //  HTML columns are sized ACCORDING to the box size.
    HomeElements = getHomeListboxElements();
    HomeListbox.string   = HomeElements("items_str")
    HomeListbox.userdata = HomeElements("items_mat")
    HomeListbox.visible = "on"  // refresh the display

    // Message Frame
    // =========================================================================

    // Frame
    msgFrame       = uicontrol( ..
    "Parent"     , atomsfig,..
    "Style"      , "frame",..
    "Relief"     , "solid",..
    "Background" , [1 1 1],..
    "Position"   , [margin margin msgWidth msgHeight],..
    "Tag"        , "msgFrame");

    // Text
    msgText          = uicontrol( ..
    "Parent"       , msgFrame,...
    "Style"        , "text",..
    "HorizontalAlignment", "left",..
    "VerticalAlignment"  , "middle",..
    "String"       , "", ..
    "FontSize"     , 12,..
    "Background"   , [1 1 1],..
    "Position"     , [2 2 msgWidth-10 msgHeight-4],..
    "Tag"          , "msgText");

endfunction

