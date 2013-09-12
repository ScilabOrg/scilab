// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2013 - Samuel GOUGEON : autoload: checkbox management
//                                       autoload status added in installed list
//                                       unrelevant buttons are now masked.
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function cbAtomsGui()

    // Load Atoms Internals lib if it's not already loaded
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    UItag = get(gcbo,"Tag");

    // Get the description frame object
    DescFrame = findobj("tag","DescFrame");

    // Display selected module information
    // =========================================================================

    if or(UItag == ["LeftListbox";"HomeListbox"]) then

        // Get the selected module
        selected = getSelected(UItag);



        if selected(1)=="module" then

            // Save the module name
            set(DescFrame,"userdata",selected(2));

            // Update the description frame
            updateDescFrame();

            // Show the description frame
            showDesc(atomsIsInstalled(selected(2)));

        elseif selected(1)=="category" then

            LeftListbox             = findobj("tag","LeftListbox");
            LeftElements            = atomsGetLeftListboxElts(selected(2));
            LeftListbox("String")   = LeftElements("items_str");
            LeftListbox("UserData") = LeftElements("items_mat");

            LeftFrame               = findobj("tag","LeftFrame");
            LeftFrame("UserData")   = selected(2);

            // Figure name
            atomsfig                = findobj("tag","atomsFigure");
            atomsfig("figure_name") = LeftElements("title")+" - ATOMS";

        end

    end

    // A button | autoload checkBox has been pressed | checked / unchecked
    // =========================================================================

    if or(UItag == ["installButton";"removeButton";"updateButton"]) then
        // Get the module name
        module = get(DescFrame,"userdata");

        // Disable callbacks
        disableAtomsGui();
    end

    // autoload checkbox Action
    // =========================================================================
    if UItag == "autoloadCheckbox" then
        module = get(DescFrame,"userdata")
        element = findobj("Tag", "autoloadCheckbox")
        if element.value==element.max then
            atomsAutoloadAdd(module)
            set(element, "Value", element.max)
            msg = _("The module will be automatically loaded for all forthcoming startup")
            msg = "<html><span style=""color:green"">"+msg+"</span>"
            updateStatusBar("info",msg+" &hellip;")
        else
            atomsAutoloadDel(module)
            set(element, "Value", element.min)
            msg = _("Autoload at startup is cancelled. The ""Toolboxes"" menu or atomsLoad() can be used to load the module when needed")
            msg = "<html><span style=""color:green"">"+msg+"</span>"
            updateStatusBar("info",msg)
        end

        // Install selected module
        // =========================================================================

    elseif UItag == "installButton" then

        updateStatusBar("info",gettext("Installing")+" &hellip;");

        if execstr("atomsInstall("""+module+""")","errcatch")<>0 then
            updateStatusBar();
            messagebox(gettext("Installation failed!"),gettext("ATOMS error"),"error");
        else
            updateDescFrame()
            msg = _("Installation done! Please restart Scilab to take changes into account.")
            updateStatusBar("success", msg);
        end
        reloadLeftListbox()
        updateInstalledListbox()

        // Remove selected module
        // =========================================================================

    elseif UItag == "removeButton" then // Remove selected module
        updateStatusBar("info",gettext("Removing")+" &hellip;");

        if execstr("atomsRemove("""+module+""")", "errcatch")<>0 then
            updateStatusBar();
            messagebox(gettext("Remove failed!"),gettext("ATOMS error"),"error");
        else
            updateDescFrame()
            msg = _("Remove done! Please restart Scilab to take changes into account. ")
            updateStatusBar("success", msg);
        end
        reloadLeftListbox()
        updateInstalledListbox()

        // Update selected module
        // =========================================================================

    elseif UItag == "updateButton" then // Update selected module

        updateStatusBar("info",gettext("Updating")+" &hellip;");

        if execstr("atomsUpdate("""+module+""")","errcatch")<>0 then
            updateStatusBar();
            messagebox(gettext("Update failed!"),gettext("ATOMS error"),"error");
        else
            updateDescFrame();
            msg = _("Update done! Please restart Scilab to take changes into account.")
            updateStatusBar("success", msg);
        end

        // Quit button => display of installed modules
        // =========================================================================

    elseif UItag == "quitButton" then
        // update the home Listbox
        HomeListbox             = findobj("tag","HomeListbox")
        HomeListbox("String")   = setInstalledList(atomsGetInstalled())

        // Erase the status bar
        updateStatusBar()

        // Show the Home page
        show("HomeFrame");
        show("HomeTitle");
        show("HomeListbox");

        // End of the button action
        // =========================================================================
    end

    // Menu
    // =========================================================================

    // File:Home
    if UItag == "homeMenu" then
        showHome();

        // File:Close
    elseif UItag == "closeMenu" then
        delete(findobj("Tag", "atomsFigure"));

        // ?:Help
    elseif UItag == "helpMenu" then
        help("atoms")

    end

endfunction

// =============================================================================
// getHomeListboxElements()
//
// Returns a struct that contains the followings fields:
//  - elements("items_str")
//  - elements("items_mat")
//
// =============================================================================

function elements = getHomeListboxElements()
    items_str  = [];
    items_mat  = [];

    installed  = atomsGetInstalled();
    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    atomsfig   = findobj("tag","atomsFigure");
    allModules = atomsfig("UserData");

    items_str  = setInstalledList(installed)
    items_mat = installed(:,1)
    items_mat = [emptystr(items_mat)+"module" items_mat ]

    if items_str==[] then
        elements("items_str") = "";
    else
        elements("items_str") = items_str;
    end

    elements("items_mat") = items_mat;

endfunction

// =============================================================================
//
// items_str = setInstalledList(installed)
//
// items_str must be updated: it contains infos about the autoload status
//  of each installed module, while this status can be interactively switched
// =============================================================================
function items_str = setInstalledList(installed)

    // Load Atoms Internals lib if it's not already loaded
    if ~exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    atomsfig   = findobj("tag","atomsFigure");
    allModules = atomsfig("UserData");

    // Widths of HTML columns
    id = findobj("Tag", "HomeListbox")
    col1Width = max([315 id.position(3)*0.83]);        // short description
    col2Width = max([60 id.position(3)-col1Width ]);   // autoload status

    items_str = []
    for i = 1:size(installed(:,1),"*")
        MRVersionAvailable = atomsGetMRVersion(installed(i,1));
        MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(installed(i,1)),"DESC");
        MRVersionInstalled = MRVersionInstalled(1);
        if atomsVersionCompare(MRVersionInstalled,MRVersionAvailable) == -1 then
            // Not up-to-date
            icon = "software-update-notinstalled.png";
        else
            // The Most Recent Version is already installed
            icon = "software-update-installed.png";
        end

        if modulo(i,2) == 0 then
            background = "#eeeeee";
        else
            background = "#ffffff";
        end

        // Building the content
        if or(installed(i,1)==autoloaded) then
            tmp = "<span style=""color:green"">"+_("loaded<br>at startup")+"</span>"
        else
            tmp = "<span style=""color:#A0A0A0;font-size:10"">"+..
            _("atomsLoad()<br>Toolboxes<br>menu")+..
            "</span>"
        end

        thisItem = [
        "<html>"
        "<table style=""background-color:"+background+";color:#000000;"">"
        "<tr>"
        "<td>"
        "<img src=""file:///"+SCI+"/modules/atoms/images/icons/16x16/status/"+icon+""" />"
        "</td>"
        "<td><div style=""width:"+sprintf("%d",col1Width*0.72)+"px;text-align:left;"">"
        "<span style=""font-weight:bold;"">"
        allModules(installed(i,1))(installed(i,2)).Title+" "
        installed(i,2)
        "</span>"
        "<br/>"
        "<span>"+allModules(installed(i,1))(installed(i,2)).Summary+"</span>"
        "<br/>"
        "<span style=""font-style:italic;"">"+installed(i,4)+"</span>"
        "</div>"
        "</td>"
        "<td style=""text-align:center;width:"+sprintf("%d",col2Width*0.72)+"px"">"
        tmp
        "</td>"
        "</tr>"
        "</table>"
        "</html>"
        ]

        items_str = [ items_str ; strcat(thisItem) ];
    end
    if items_str==[] then
        items_str = ""
    end
endfunction

// =============================================================================
// getSelected()
//  + Return the type: category / module
//  + Return the name selected from a listbox.
// =============================================================================

function selected = getSelected(listbox)
    index    = get(findobj("Tag",listbox),"Value");
    UserData = get(findobj("Tag",listbox),"UserData");
    selected = UserData(index,:);

endfunction

// =============================================================================
// disableAtomsGui()
//  + Disable all callback
// =============================================================================

function disableAtomsGui()
    set(findobj("tag", "installButton"), "Enable", "off");
    set(findobj("tag", "updateButton") , "Enable", "off");
    set(findobj("tag", "removeButton") , "Enable", "off");
    disableLeftListbox()
endfunction

function disableLeftListbox()
    set(findobj("tag", "LeftListbox"),"Callback", "");
    set(findobj("tag", "LeftListbox"),"ForegroundColor",[0.5 0.5 0.5]);
endfunction

function enableLeftListbox()
    set(findobj("tag", "LeftListbox"),"Callback", "cbAtomsGui");
    set(findobj("tag", "LeftListbox"),"ForegroundColor",[0 0 0]);
endfunction

// =============================================================================
// reloadLeftListbox()
// =============================================================================

function reloadLeftListbox()
    category                = get(findobj("tag","LeftFrame"),"UserData")
    LeftListbox             = findobj("tag","LeftListbox")
    LeftElements            = atomsGetLeftListboxElts(category)
    LeftListbox("String")   = LeftElements("items_str")
    LeftListbox("UserData") = LeftElements("items_mat")
    LeftListbox.Callback    = "cbAtomsGui"
endfunction

// =============================================================================
// updateInstalledListbox()
// =============================================================================

function updateInstalledListbox()
    InstalledElements         = getHomeListboxElements()
    InstalledListbox          = findobj("tag","HomeListbox")
    InstalledListbox.String   = InstalledElements("items_str")
    InstalledListbox.UserData = InstalledElements("items_mat")
endfunction

// =============================================================================
// updateDescFrame()
//  + Update the description frame with the selected module
//  + does not change the description frame visibility
// =============================================================================

function updateDescFrame()

    // Operating system detection + Architecture detection
    // =========================================================================
    [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform();

    // Get the modules list and the selected module
    // =========================================================================

    thisFigure     = findobj("tag","atomsFigure");

    Desc           = findobj("tag","Desc");
    DescFrame      = findobj("tag","DescFrame");
    DescTitle      = findobj("tag","DescTitle");

    allModules     = get(thisFigure,"userdata");
    thisModuleName = get(DescFrame ,"userdata");

    // Reset the message frame
    // =========================================================================
    set(findobj("tag","msgText"),"String","");

    // Get the module details
    // =========================================================================

    modulesNames       = getfield(1, allModules);
    modulesNames (1:2) = [];
    thisModuleStruct   = allModules(thisModuleName);

    MRVersionAvailable = atomsGetMRVersion(thisModuleName);
    MRVersionInstalled = "";

    if atomsIsInstalled(thisModuleName) then
        MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(thisModuleName),"DESC");
        MRVersionInstalled = MRVersionInstalled(1);
        thisModuleDetails  = thisModuleStruct(MRVersionInstalled);
    else
        thisModuleDetails  = thisModuleStruct(MRVersionAvailable);
    end

    // Download Size
    // =========================================================================

    sizeHTML = "";

    if isfield(thisModuleDetails,OSNAME+ARCH+"Size") then
        sizeHTML = txt2title(gettext("Download size")) ..
        + "<div>" ..
        + atomsSize2human(thisModuleDetails(OSNAME+ARCH+"Size")) ..
        + "</div>";
    end

    // Authors
    // =========================================================================

    authorMat  = thisModuleDetails.Author;
    authorHTML = "";

    for i=1:size(authorMat,"*")
        authorHTML = authorHTML + authorMat(i)+"<br>";
    end

    authorHTML = txt2title(gettext("Author(s)")) ..
    + "<div>" ..
    + authorHTML
    + "</div>";

    // URLs (See also)
    // =========================================================================

    URLs        = [];
    seeAlsoHTML = "";

    if isfield(thisModuleDetails,"URL") & (thisModuleDetails.URL<>"") then
        URLs = [ URLs ; thisModuleDetails.URL ];
    end

    if isfield(thisModuleDetails,"WebSite") & (thisModuleDetails.WebSite<>"") then
        URLs = [ URLs ; thisModuleDetails.WebSite ];
    end

    if ~isempty(URLs) then

        for i=1:size(URLs,"*")
            seeAlsoHTML = seeAlsoHTML + "&nbsp;&bull;&nbsp;"+URLs(i)+"<br>";
        end

        seeAlsoHTML = txt2title(gettext("See also"))..
        + "<div>" ..
        + seeAlsoHTML ..
        + "</div>";
    end

    // Release date
    // =========================================================================

    dateHTML = "";

    if isfield(thisModuleDetails,"Date") ..
        & ~isempty(regexp(thisModuleDetails.Date,"/^[0-9]{4}-[0-1][0-9]-[0-3][0-9]\s/")) then

        dateHTML = txt2title(gettext("Release date")) ..
        + "<div>" ..
        + part(thisModuleDetails.Date,1:10) ..
        + "</div>";
    end

    // Build and Set the HTML code
    // =========================================================================


    htmlcode = "<html>" + ..
    "<body>" + ..
    txt2title(gettext("Version")) + ..
    "<div>" + thisModuleDetails.Version  + "</div>" + ..
    authorHTML + ..
    txt2title(gettext("Description")) + ..
    "<div>" + ..
    strcat(thisModuleDetails.Description,"<br>")  + ..
    "</div>" + ..
    seeAlsoHTML + ..
    dateHTML + ..
    sizeHTML + ..
    "</body>" + ..
    "</html>";

    // Process URLs and Emails
    htmlcode = processHTMLLinks(htmlcode);

    // Update the main description
    set(Desc,"String",htmlcode);

    // Description title management
    // =========================================================================

    descFrameHTML    = thisModuleDetails.Title;

    descFramePos     = get(DescTitle,"Position");
    descFramePos(3)  = 300;

    set(DescTitle, "String"   , descFrameHTML );
    set(DescTitle, "Position" , descFramePos );

    // Buttons
    // =========================================================================

    // Tests for update available
    // --------------------------

    canUpdate = "off";

    if atomsIsInstalled(thisModuleName) & atomsVersionCompare(MRVersionInstalled,MRVersionAvailable) == -1 then
        // Not up-to-date
        canUpdate = "on";
        updateStatusBar("warning",sprintf(gettext("A new version (''%s'') of ''%s'' is available"),MRVersionAvailable,thisModuleDetails.Title));
    end

    // Is autoloaded
    // -------------
    tmp = atomsAutoloadList()
    val = bool2s(or(thisModuleName==tmp))
    set(findobj("tag", "autoloadCheckbox"), "Value", val)

    // Can be removed
    // --------------------------

    if atomsIsInstalled(thisModuleName) then
        canRemove = "on";
    else
        canRemove = "off";
    end

    // Can be installed
    // --------------------------

    if ~ atomsIsInstalled(thisModuleName) then
        canInstall = "on";
    else
        canInstall = "off";
    end

    // Update the buttons
    // --------------------------

    set(findobj("tag", "installButton"), "Enable", canInstall);
    set(findobj("tag", "updateButton") , "Enable", canUpdate );
    set(findobj("tag", "removeButton") , "Enable", canRemove );

endfunction

// =============================================================================
// atomsSize2human()
// =============================================================================

function human_str = atomsSize2human(size_str)

    size_int = strtod(size_str);

    if size_int < 1024 then
        human_str = string(size_int) + " " + gettext("Bytes");

    elseif size_int < 1024*1024 then
        human_str = string(round(size_int/1024)) + " " + gettext("KB");

    else
        human_str = string( round((size_int*10)/(1024*1024)) / 10 ) + " " + gettext("MB");

    end

endfunction

// =============================================================================
// show()
// =============================================================================

function show(tag)
    obj = findobj("tag",tag);
    if obj~=[]
        set(obj,"Visible","On");
    end
endfunction

// =============================================================================
// hide()
// =============================================================================

function hide(tag)
    obj = findobj("tag",tag);
    if obj~=[]
        set(obj,"Visible","Off");
    end
endfunction

// =============================================================================
// showHome()
// + Hide the detailed description of a module
// + Show the home page
// =============================================================================

function showHome()

    // Reset the message frame
    set(findobj("tag","msgText"),"String","");

    // Hide the Desc frame
    hide("DescFrame");
    hide("DescTitle");
    hide("Desc");
    hide("removeButton");
    hide("installButton");
    hide("updateButton");
    hide("quitButton");

    // update the left listbox
    LeftListbox             = findobj("tag","LeftListbox");
    LeftElements            = atomsGetLeftListboxElts("filter:main");
    LeftListbox("String")   = LeftElements("items_str");
    LeftListbox("UserData") = LeftElements("items_mat");

    // update the home Listbox
    HomeListbox             = findobj("tag","HomeListbox")
    HomeListbox("String")   = setInstalledList(atomsGetInstalled())

    // Show the Home page
    show("HomeFrame");
    show("HomeTitle");
    show("HomeListbox");

endfunction

// =============================================================================
// showDesc()
// + Hide the home page
// + Show the detailed description of a module
// =============================================================================

function showDesc(is_installed)

    // Show the Home page
    hide("HomeFrame");
    hide("HomeTitle");
    hide("HomeListbox");

    // Show the Desc frame
    show("DescFrame");
    show("DescTitle");
    show("Desc");

    show("quitButton")
    if is_installed
        show("autoloadCheckbox")
        show("updateButton")
        show("removeButton")
        hide("installButton")
    else
        hide("autoloadCheckbox")
        hide("updateButton")
        hide("removeButton")
        show("installButton")
    end

endfunction


// =============================================================================
// updateStatusBar
// + Update the string in the msg Frame
// =============================================================================

function updateStatusBar(status,msg)

    rhs = argn(2);

    msgText = findobj("tag","msgText");

    if rhs==0 then
        set(msgText,"String","");
        return
    end

    select status
    case "warning" then
        fontcolor = "#ff0000"; // red
        icon      = "status/software-update-available.png";
    case "success" then
        fontcolor = "#009a1b"; // dark green
        icon      = "emblems/emblem-default.png";
    case "info" then
        fontcolor = "#7d7d7d"; // dark green
        icon      = "status/dialog-information.png";
    end

    str =       "<html>";
    str = str + "<table><tr>";
    str = str + "<td><img src=""file:///"+SCI+"/modules/atoms/images/icons/16x16/"+icon+""" /></td>";
    str = str + "<td><div style=""color:"+fontcolor+";font-style:italic;"">"+msg+"</div></td>";
    str = str + "</tr></table>";
    str = str + "</html>";

    set(msgText,"String",str);

endfunction

// =============================================================================
// processHTMLLinks
// + Find URLs
// + Convert them in HTML (hyperlinks)
// =============================================================================

function txtout = processHTMLLinks(txtin)

    regexUrl   = "/((((H|h)(T|t)|(F|f))(T|t)(P|p)((S|s)?))\:\/\/)(www|[a-zA-Z0-9])[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\:[0-9]{1,5})*(\/($|[a-zA-Z0-9\.\,\;\?\''\\\+&amp;%\$#\=~_\-\/]+))*/";
    txtout     = "";

    [mat_start,mat_end,mat_match] = regexp(txtin,regexUrl);

    if ~isempty(mat_match) then
        mat_end = [ 0 mat_end ];
        for i=1:size(mat_match,"*")
            txtout = txtout + part(txtin,[mat_end(i)+1:mat_start(i)-1]) ..
            + "<a href="""+mat_match(i)+""" target=""_blank"">" ..
            + mat_match(i) ..
            + "</a>";
        end
        txtout = txtout + part(txtin,mat_end(size(mat_end,"*"))+1:length(txtin));
    else
        txtout = txtin;
    end

endfunction

// =============================================================================
// txt2title
// =============================================================================

function txtout = txt2title(txtin)

    txtout = "<div style=""font-weight:bold;margin-top:10px;margin-bottom:3px;"">" + ..
    txtin + ..
    "</div>";

endfunction

