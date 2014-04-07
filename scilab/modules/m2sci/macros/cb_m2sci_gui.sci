//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

// Callback function for M2SCI GUI (See m2sci_gui.sci)

function cb_m2sci_gui

    //
    // --- Switch to file conversion mode ---
    //
    if get(gcbo,"tag")=="fileradio" then

        set(gcbo, "value", 1);
        set("dirradio", "value", 0);

        // Make dir objects not-visible
        set("dirlabel", "visible","off");
        set("diredit", "visible","off");
        set("dirbtn", "visible","off");

        // Make file objects visible
        set("filedirselect", "border",createBorder("titled", createBorder("line", "lightGray", 1),...
        _("Input file"),"left","top",createBorderFont("", 11, "normal"), "black"));
        set("filelabel", "visible","on");
        set("fileedit", "visible","on");
        set("filebtn", "visible","on");

        //
        // --- Switch to directory conversion mode ---
        //
    elseif get(gcbo,"tag")=="dirradio" then

        set(gcbo, "value", 1);
        set("fileradio", "value", 0);

        // Make dir objects visible
        set("filedirselect", "border",createBorder("titled", createBorder("line", "lightGray", 1),...
        _("Input directory"),"left","top",createBorderFont("", 11, "normal"), "black"));
        set("dirlabel", "visible", "on");
        set("diredit", "visible", "on");
        set("dirbtn", "visible", "on");

        // Make file objects not-visible
        set("filelabel", "visible", "off");
        set("fileedit", "visible", "off");
        set("filebtn", "visible", "off");

        //
        // --- Directory to convert ---
        //
    elseif get(gcbo,"tag")=="dirbtn" then

        directoryname = uigetdir(pwd(), gettext("Select the directory to convert"));
        set("diredit", "string", directoryname);

        //
        // --- File to convert ---
        //
    elseif get(gcbo,"tag")=="filebtn" then

        filename = uigetfile("*.m", pwd(), gettext("Select the file to convert"));
        set("fileedit", "string", filename);

        //
        // --- Output directory ---
        //
    elseif get(gcbo,"tag")=="outbtn" then

        directoryname = uigetdir(pwd(), gettext("Select the directory for generated files"));
        set("outedit", "string", directoryname);

        //
        // --- Recmode option ---
        //
    elseif or(get(gcbo, "tag")==["recradioyes","recradiono"]) then

        set("recradioyes", "value", 0);
        set("recradiono", "value", 0);

        set(gcbo, "value", 1);

        //
        // --- Double option ---
        //
    elseif or(get(gcbo, "tag")==["doubradioyes","doubradiono"]) then

        set("doubradioyes", "value", 0);
        set("doubradiono", "value", 0);

        set(gcbo, "value", 1);

        //
        // --- Verbose option ---
        //
    elseif or(get(gcbo, "tag")==["verbradio0","verbradio1","verbradio2","verbradio3"]) then

        set("verbradio0", "value", 0);
        set("verbradio1", "value", 0);
        set("verbradio2", "value", 0);
        set("verbradio3", "value", 0);

        set(gcbo, "value", 1);

        //
        // --- Pretty print option ---
        //
    elseif or(get(gcbo, "tag")==["ppradioyes","ppradiono"]) then

        set("ppradioyes", "value", 0);
        set("ppradiono", "value", 0);

        set(gcbo, "value", 1);

        //
        // --- Cancel conversion ---
        //
    elseif get(gcbo, "tag")=="cancelbtn" | get(gcbo, "tag")=="close_menu" then
        //delete(findobj("tag", "m2scifig"));
        delete(gcf())

        //
        // --- Launch conversion ---
        //
    elseif get(gcbo, "tag")=="convertbtn" then
        outputdir = get("outedit", "string");

        rec = get("recradioyes", "value") == 1;

        doub = get("doubradioyes", "value") == 1;

        if get("verbradio0", "value") == 1 then
            verb = 0;
        elseif get("verbradio1", "value") == 1 then
            verb = 1;
        elseif get("verbradio2", "value") == 1 then
            verb = 2;
        else
            verb = 3;
        end

        pp = get("ppradioyes", "value") == 1;

        if get("fileradio", "value") == 1 then // Single file conversion
            inputfile = get("fileedit", "string");
            //delete(findobj("tag", "m2scifig"));
            delete(gcf());
            mfile2sci(inputfile, outputdir, rec, doub, verb, pp);
        else // Directory conversion
            inputdir = get("diredit", "string");
            //delete(findobj("tag", "m2scifig"));
            delete(gcf());
            translatepaths(inputdir, outputdir);
        end

        //
        // --- mfile2sci help ---
        //
    elseif get(gcbo, "tag")=="mfile2sci_help_menu" then
        help mfile2sci

        //
        // --- translatepaths help ---
        //
    elseif get(gcbo, "tag")=="translatepaths_help_menu" then
        help translatepaths

        //
        // --- About M2SCI ---
        //
    elseif get(gcbo, "tag")=="about_m2sci_menu" then
        help(gettext("About_M2SCI_tools"))
    end

endfunction
