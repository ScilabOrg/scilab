// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function ged(k,win)

    if ~%tk then
        warning(msprintf(gettext("Tcl/Tk interface not installed.")));
        return
    end

    // Check number of arguments
    if argn(2) ==1 then
        win=get(gcf(),"figure_id")
    elseif argn(2)<>2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "ged", 2));
    end

    // Check argument #1
    if typeof(k) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A Real expected.\n"), "ged", 1));
    end
    if size(k, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A Real expected.\n"), "ged", 1));
    end

    // Check argument #2
    if typeof(win) <> "constant" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A Real expected.\n"), "ged", 2));
    end
    if size(win, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A Real expected.\n"), "ged", 2));
    end

    global ged_current_figure
    global ged_cur_fig_handle

    ged_current_figure=gcf(); //Preserve current figure

    ged_cur_fig_handle=scf(win);
    show_window(ged_cur_fig_handle);

    // for TCL input args built with string(). See bug http://bugzilla.scilab.org/2479
    initFormat = format()
    format("v",18)  // To be restored with initFormat before leaving

    if k>3 then
        TCL_EvalStr("set isgedinterp [interp exists ged]")
        if ~TCL_ExistInterp( "ged" ) then
            TCL_CreateSlave( "ged" ) ;
        end

    end

    select k
    case 1 then //Select (make it current)
        format(initFormat(2),initFormat(1))
        return
    case 2 then //redraw
        // nothing to do in new graphic mode
    case 3 then //erase
        clf()
    case 4 then //copy
        ged_copy_entity()
    case 5 then //past
        ged_paste_entity()
    case 6 then //move
        ged_move_entity()
    case 7 then //Delete Entity
        ged_delete_entity()

    case 8 then //edit current figure properties

        // hierarchical viewer
        TK_send_handles_list(ged_cur_fig_handle)
        TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(ged_cur_fig_handle)))

        //color_map array for color sample display
        f=ged_cur_fig_handle;
        for i=1:size(f.color_map,1)
            redname= "RED("+string(i)+")";
            TCL_EvalStr("set "+redname+" "+string(f.color_map(i,1)));
            grename= "GREEN("+string(i)+")";
            TCL_EvalStr("set "+grename+" "+string(f.color_map(i,2)));
            bluname= "BLUE("+string(i)+")";
            TCL_EvalStr("set "+bluname+" "+string(f.color_map(i,3)));
        end

        TCL_SetVar("msdos",string(getos() == "Windows")) // to know the OS

        // get the number of the window associated with ged
        TCL_SetVar("sciGedIsAlive",string(ged_cur_fig_handle.figure_id)) ;
        TCL_SetVar("SCIHOME", SCIHOME) // to know the home directory to put temporary files

        ged_figure(ged_cur_fig_handle)
    case 9 then //edit current axes

        // hierarchical viewer
        TK_send_handles_list(ged_cur_fig_handle)
        TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(gca())))

        //color_map array for color sample display
        f=ged_cur_fig_handle;
        for i=1:size(f.color_map,1)
            redname= "RED("+string(i)+")";
            TCL_EvalStr("set "+redname+" "+string(f.color_map(i,1)));
            grename= "GREEN("+string(i)+")";
            TCL_EvalStr("set "+grename+" "+string(f.color_map(i,2)));
            bluname= "BLUE("+string(i)+")";
            TCL_EvalStr("set "+bluname+" "+string(f.color_map(i,3)));
        end

        TCL_SetVar("msdos",string(getos() == "Windows")) // to know the OS

        // get the number of the window associated with ged
        TCL_SetVar("sciGedIsAlive",string(ged_cur_fig_handle.figure_id)) ;
        TCL_SetVar("SCIHOME", SCIHOME) // to know the home directory to put temporary files

        ged_axes(gca())
    case 10 then //start Entity picker
        fig=ged_cur_fig_handle
        fig_ud=get(fig,"user_data")
        if fig.event_handler<>""& fig.event_handler<>"ged_eventhandler" then
            //push current event handler in fig user data if possible
            if fig_ud==[] then fig_ud=struct();end
            if typeof(fig_ud)=="st" then
                if ~isfield(fig_ud,"handlers") then  fig_ud.handlers=[],end
                fig_ud.handlers=[fig_ud.handlers;
                fig.event_handler fig.event_handler_enable]
                set(fig,"user_data",fig_ud)
            else
                warning(_("Entity picker cannot be enabled, user data figure field is already used" ))
                return
            end
            fig.event_handler_enable = "off" //to prevent against bug 7855
            fig.event_handler="ged_eventhandler"
            fig.event_handler_enable="on"
        end
        ged_cur_fig_handle.info_message=_("Left click on a graphic entity to open its property editor");
    case 11 then //stop Entity picker
        fig=ged_cur_fig_handle
        fig_ud=get(fig,"user_data")
        seteventhandler("")
        fig.event_handler_enable = "off"
        if typeof(fig_ud)=="st"&isfield(fig_ud,"handlers")&fig_ud.handlers<>[] then
            fig.event_handler=fig_ud.handlers($,1)
            fig.event_handler_enable=fig_ud.handlers($,2)
            fig_ud.handlers= fig_ud.handlers(1:$-1,:)
            set(fig,"user_data",fig_ud)
        else
            fig.event_handler_enable = "off"
        end
    end
    scf(ged_current_figure)
    format(initFormat(2), initFormat(1))
endfunction


function curgedindex_ = Get_handle_pos_in_list(h)
    global ged_cur_fig_handle

    curgedindex_ = [];

    handles = Get_handles_list(ged_cur_fig_handle)
    for i=1:size(handles,1)
        if (h==handles(i))
            curgedindex_ = i;
        end
    end

    // Other case :
    // a label has been selected (and for now they are included inside the Axes)
    if (curgedindex_==[])
        if h.type == "Label"
            h = h.parent;
            for i=1:size(handles,1)
                if (h==handles(i))
                    curgedindex_ = i;
                end
            end
        end
    end

endfunction


// Search the depth level for each handle
// Usefull for new hierarchical graphic tree.
function ged_levels = Get_levels(handles);

    ged_levels = 1; // for Figure, always 1

    f = handles(1);

    for i=2:size(handles,1)
        ged_levels(i) = Get_Depth(f,handles(i));
    end

    //disp("les levels sont:")xb

    //disp(ged_levels);

endfunction

function depth = Get_Depth(f,h)

    depth = 2;

    while  h.parent <> f
        h = h.parent;
        depth = depth + 1;
    end

endfunction



function TK_send_handles_list(h)
    iFig = 0;
    iUim = 0; // uimenu
    iUic = 0; // uicontrol
    iAxe = 0; // <=> subwindow in C code
    iAgr = 0;
    iPol = 0;
    iPl3 = 0;
    iFac = 0;
    iRec = 0;
    iTex = 0;
    iLeg = 0;
    iArc = 0;
    iSeg = 0;
    iCha = 0; // Champ
    iFec = 0;
    iGra = 0;
    iMat = 0; // forgotten object F.Leray 22.10.04
    iAxi = 0; // axis : entity created when using drawaxis method for example
    iLab = 0;

    f=getparfig(h);
    handles = Get_handles_list(f)

    ged_levels = Get_levels(handles);

    TCL_SetVar("ged_handle_list_size",string(size(handles,1)));

    for i=1:size(handles,1)
        SelObject="LEVELS("+string(i)+")";
        TCL_EvalStr("set "+SelObject+" "+string(ged_levels(i)));
    end


    for i=1:size(handles,1)
        SelObject="SELOBJECT("+string(i)+")";
        hand = handles(i);
        select  hand.type
        case "Figure"
            iFig = iFig+1;
            figname= "Figure("+string(iFig)+")";
            TCL_EvalStr("set "+SelObject+" "+figname);
        case "uimenu"
            iUim = iUim+1;
            uimname= "Uimenu("+string(iUim)+")";
            TCL_EvalStr("set "+SelObject+" "+uimname);
        case "uicontrol"
            iUic = iUic+1;
            uicname= "Uicontrol("+string(iUic)+")";
            TCL_EvalStr("set "+SelObject+" "+uicname);
        case "Axes"
            iAxe = iAxe+1;
            axename= "Axes("+string(iAxe)+")";
            TCL_EvalStr("set "+SelObject+" "+axename);
            //   case "Label"  // to see later: have labels at the same level than Axes (to have a better visibility)
            //    iLab = iLab+1;
            //    labname= "Label("+string(iLab)+")";
            //    TCL_EvalStr('set '+SelObject+" "+labname);
        case "Compound"
            iAgr = iAgr+1;
            agrname= "Compound("+string(iAgr)+")";
            TCL_EvalStr("set "+SelObject+" "+agrname);
        case "Polyline"
            iPol = iPol+1;
            polname= "Polyline("+string(iPol)+")";
            TCL_EvalStr("set "+SelObject+" "+polname);
        case "Plot3d"
            iPl3 = iPl3+1;
            pl3name= "Plot3d("+string(iPl3)+")";
            TCL_EvalStr("set "+SelObject+" "+pl3name);
        case "Fac3d"
            iFac = iFac+1;
            Facname= "Fac3d("+string(iFac)+")";
            TCL_EvalStr("set "+SelObject+" "+Facname);
        case "Rectangle"
            iRec = iRec+1;
            Recname= "Rectangle("+string(iRec)+")";
            TCL_EvalStr("set "+SelObject+" "+Recname);
        case "Text"
            iTex = iTex+1;
            Texname= "Text("+string(iTex)+")";
            TCL_EvalStr("set "+SelObject+" "+Texname);
        case "Legend"
            iLeg = iLeg+1;
            Legname= "Legend("+string(iLeg)+")";
            TCL_EvalStr("set "+SelObject+" "+Legname);
        case "Arc"
            iArc = iArc+1;
            Arcname= "Arc("+string(iArc)+")";
            TCL_EvalStr("set "+SelObject+" "+Arcname);
        case "Segs"
            iSeg = iSeg+1;
            Segname= "Segs("+string(iSeg)+")";
            TCL_EvalStr("set "+SelObject+" "+Segname);
        case "Champ"
            iCha = iCha+1;
            Chaname= "Champ("+string(iCha)+")";
            TCL_EvalStr("set "+SelObject+" "+Chaname);
        case "Fec"
            iFec = iFec+1;
            Fecname= "Fec("+string(iFec)+")";
            TCL_EvalStr("set "+SelObject+" "+Fecname);
        case "Grayplot"
            iGra = iGra+1;
            Graname= "Grayplot("+string(iGra)+")";
            TCL_EvalStr("set "+SelObject+" "+Graname);
        case "Matplot"
            iMat = iMat+1;
            Matname= "Matplot("+string(iMat)+")";
            TCL_EvalStr("set "+SelObject+" "+Matname);
        case "Axis"
            iAxi = iAxi+1;
            Axiname= "Axis("+string(iAxi)+")";
            TCL_EvalStr("set "+SelObject+" "+Axiname);
        else
            error( hand.type + " not handled");
        end
    end
endfunction

////////////////////////////////////////////
function  hfig= getparfig( h )

    htmp = h;
    hfig= []

    while htmp.type<>"Figure" do
        htmp=htmp.parent
    end

    hfig = htmp;
endfunction



function ged_Compound(h)
    global ged_handle;ged_handle=h;
    TCL_SetVar("curvis",h.visible)
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Compound.tcl")
endfunction


function ged_figure(h)
    global ged_handle;ged_handle=h;
    TCL_SetVar("background",string(h.background))
    TCL_SetVar("rotation_style",h.rotation_style)
    TCL_SetVar("figure_name",h.figure_name)
    TCL_SetVar("figure_id",string(h.figure_id))
    TCL_SetVar("figure_xposition",string(h.figure_position(1)))
    TCL_SetVar("figure_yposition",string(h.figure_position(2)))
    TCL_SetVar("figure_xsiz",string(h.figure_size(1)))
    TCL_SetVar("figure_ysiz",string(h.figure_size(2)))
    TCL_SetVar("figure_xaxesiz",string(h.axes_size(1)))
    TCL_SetVar("figure_yaxesiz",string(h.axes_size(2)))
    TCL_SetVar("bcolor",string(h.background))
    TCL_SetVar("ncolors",string(size(h.color_map,1)))
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curpdm",h.pixel_drawing_mode)
    TCL_SetVar("curautoresize",h.auto_resize)
    TCL_SetVar("currotation_style",h.rotation_style)
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Figure.tcl")
endfunction


function ged_axes(h)
    global ged_handle;ged_handle=h;

    LoadTicks2TCL(h);

    TCL_SetVar("Xaxes_reverseToggle",h.axes_reverse(1))
    TCL_SetVar("Yaxes_reverseToggle",h.axes_reverse(2))
    TCL_SetVar("Zaxes_reverseToggle",h.axes_reverse(3))

    // forgotten axes bounds info.
    TCL_SetVar("axes_boundsL",string(h.axes_bounds(1,1)))
    TCL_SetVar("axes_boundsU",string(h.axes_bounds(1,2)))
    TCL_SetVar("axes_boundsW",string(h.axes_bounds(1,3)))
    TCL_SetVar("axes_boundsH",string(h.axes_bounds(1,4)))

    // forgotten visibility info.
    TCL_SetVar("xlabel_visibility",string(h.x_label.visible))
    TCL_SetVar("ylabel_visibility",string(h.y_label.visible))
    TCL_SetVar("zlabel_visibility",string(h.z_label.visible))
    TCL_SetVar("titlelabel_visibility",string(h.title.visible))

    TCL_SetVar("Lmargins",string(h.margins(1)));
    TCL_SetVar("Rmargins",string(h.margins(2)));
    TCL_SetVar("Tmargins",string(h.margins(3)));
    TCL_SetVar("Bmargins",string(h.margins(4)));
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);
    TCL_SetVar("curautoclear",h.auto_clear);
    TCL_SetVar("curautoscale",h.auto_scale);
    //TCL_SetVar("curfillmode",h.fill_mode);
    TCL_SetVar("curalpharotation",string(h.rotation_angles(1)))
    TCL_SetVar("curthetarotation",string(h.rotation_angles(2)))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("Xlabelpos",h.x_location)
    TCL_SetVar("Ylabelpos",h.y_location)
    TCL_SetVar("Xlabelfontstyle",ged_fontarray(h.x_label.font_style+1))
    TCL_SetVar("Ylabelfontstyle",ged_fontarray(h.y_label.font_style+1))
    TCL_SetVar("Zlabelfontstyle",ged_fontarray(h.z_label.font_style+1))
    TCL_SetVar("TITLEfontstyle",ged_fontarray(h.title.font_style+1))
    TCL_SetVar("fontstyle",ged_fontarray(h.font_style+1))

    // label texts
    if size(h.x_label.text,"*") == 1 then
        txt = """" + h.x_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.x_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("xlabel", txt );

    if size(h.y_label.text,"*") == 1 then
        txt = """" + h.y_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.y_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("ylabel", txt );

    if size(h.z_label.text,"*") == 1 then
        txt = """" + h.z_label.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.z_label.text)),"x")+" string array]" ;
    end
    TCL_SetVar("zlabel", txt );

    if size(h.title.text,"*") == 1 then
        txt = """" + h.title.text + """" ;
    else
        // an array of strings
        txt = "["+strcat(string(size(h.title.text)),"x")+" string array]" ;
    end
    TCL_SetVar("tlabel", txt );

    TCL_SetVar("xlabel_fontforeground",string(h.x_label.font_foreground))
    TCL_SetVar("ylabel_fontforeground",string(h.y_label.font_foreground))
    TCL_SetVar("zlabel_fontforeground",string(h.z_label.font_foreground))
    TCL_SetVar("titlelabel_fontforg",string(h.title.font_foreground))
    TCL_SetVar("xlabel_foreground",string(h.x_label.foreground))
    TCL_SetVar("ylabel_foreground",string(h.y_label.foreground))
    TCL_SetVar("zlabel_foreground",string(h.z_label.foreground))
    TCL_SetVar("titlelabel_foreground",string(h.title.foreground))
    TCL_SetVar("xlabel_background",string(h.x_label.background))
    TCL_SetVar("ylabel_background",string(h.y_label.background))
    TCL_SetVar("zlabel_background",string(h.z_label.background))
    TCL_SetVar("titlelabel_background",string(h.title.background))
    TCL_SetVar("x_position",sci2exp(h.x_label.position,0))
    TCL_SetVar("y_position",sci2exp(h.y_label.position,0))
    TCL_SetVar("z_position",sci2exp(h.z_label.position,0))
    TCL_SetVar("title_position",sci2exp(h.title.position,0))
    TCL_SetVar("xauto_position",h.x_label.auto_position)
    TCL_SetVar("yauto_position",h.y_label.auto_position)
    TCL_SetVar("zauto_position",h.z_label.auto_position)
    TCL_SetVar("titleauto_position",h.title.auto_position)
    TCL_SetVar("xauto_rotation",h.x_label.auto_rotation)
    TCL_SetVar("yauto_rotation",h.y_label.auto_rotation)
    TCL_SetVar("zauto_rotation",h.z_label.auto_rotation)
    TCL_SetVar("titleauto_rotation",h.title.auto_rotation)
    TCL_SetVar("Xfillmode",h.x_label.fill_mode)
    TCL_SetVar("Yfillmode",h.y_label.fill_mode)
    TCL_SetVar("Zfillmode",h.z_label.fill_mode)
    TCL_SetVar("Titlefillmode",h.title.fill_mode)
    TCL_SetVar("xlabel_fontsize",string(h.x_label.font_size))
    TCL_SetVar("ylabel_fontsize",string(h.y_label.font_size))
    TCL_SetVar("zlabel_fontsize",string(h.z_label.font_size))
    TCL_SetVar("titlelabel_fontsize",string(h.title.font_size))
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("fcolor",string(h.foreground))
    TCL_SetVar("bcolor",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curfontcolor",string(h.font_color))
    TCL_SetVar("limToggle",h.tight_limits(1))
    TCL_SetVar("isoToggle",h.isoview)
    TCL_SetVar("cubToggle",h.cube_scaling)
    TCL_SetVar("viewToggle",h.view)
    TCL_SetVar("curBoxState",h.box)
    TCL_SetVar("xToggle",part(h.log_flags,1))
    TCL_SetVar("yToggle",part(h.log_flags,2))
    TCL_SetVar("zToggle",part(h.log_flags,3))
    TCL_SetVar("xGrid",string(h.grid(1)))
    TCL_SetVar("yGrid",string(h.grid(2)))

    TCL_SetVar("hiddenAxisColor",h.hidden_axis_color)
    TCL_SetVar("curfontangle_x",string(h.x_label.font_angle))
    TCL_SetVar("curfontangle_y",string(h.y_label.font_angle))
    TCL_SetVar("curfontangle_z",string(h.z_label.font_angle))
    TCL_SetVar("curfontangle_title",string(h.title.font_angle))


    select h.view
    case "2d"
        drawlater();
        h.view="3d"
        TCL_SetVar("old_curalpharotation",string(h.rotation_angles(1)))
        TCL_SetVar("old_curthetarotation",string(h.rotation_angles(2)))
        TCL_SetVar("zGrid",string(h.grid(3)))
        TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
        TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
        TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
        TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
        TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
        TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
        TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
        h.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("zGrid",string(h.grid(3)))
        TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
        TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
        TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
        TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
        TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
        TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
        TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
    end

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Axes.tcl")
endfunction

function ged_rectangle(h)
    global ged_handle; ged_handle=h
    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    ax=h;while stripblanks(ax.type)<>"Axes" then ax=ax.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curvis",h.visible)
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];
    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))


    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curfillmode",h.fill_mode)
    // Rectangle data
    select ax.view
    case "2d"
        drawlater();
        ax.view="3d"
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
        ax.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Rectangle.tcl")
endfunction

function ged_polyline(h)
    global ged_handle; ged_handle=h

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curarrowsizefactor",string(h.arrow_size_factor))
    TCL_SetVar("curvis",h.visible)

    ged_polylinestylearray=["interpolated" "staircase" "barplot" "arrowed" "filled" "bar"];
    TCL_SetVar("curpolylinestyle",ged_polylinestylearray(max(h.polyline_style,1)))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))

    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curclosedmode",h.closed)
    TCL_SetVar("curfillmode",h.fill_mode)
    TCL_SetVar("curinterpcolormode",h.interp_color_mode)
    TCL_SetVar("curinterpcolorvector",sci2exp(h.interp_color_vector,0))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    select get(getparaxe(h),"view")
    case "2d"
        TCL_SetVar("nbcol",string(2));
    case "3d"
        TCL_SetVar("nbcol",string(3));
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Polyline.tcl")
endfunction


function ged_plot3d(h)
    global ged_handle; ged_handle=h

    //  if (h.clip_box==[])
    //    TCL_SetVar("old_Xclipbox","")
    //    TCL_SetVar("old_Yclipbox","")
    //    TCL_SetVar("old_Wclipbox","")
    //    TCL_SetVar("old_Hclipbox","")
    //    TCL_SetVar("Xclipbox","")
    //    TCL_SetVar("Yclipbox","")
    //    TCL_SetVar("Wclipbox","")
    //    TCL_SetVar("Hclipbox","")
    //   else
    //    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    //    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    //  end
    //  TCL_SetVar("curclipstate",h.clip_state);

    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curcolormode",string(h.color_mode))
    TCL_SetVar("colorflagToggle",string(h.color_flag))
    TCL_SetVar("curforeground",string(h.foreground))
    TCL_SetVar("curhiddencolor",string(h.hiddencolor))
    TCL_SetVar("curthick",string(h.thickness))

    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))
    TCL_SetVar("curlinemode",h.surface_mode)


    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.z)),"x")+" double array]"
    TCL_SetVar("curdata_z",d);

    TCL_EvalStr("set flagCOLOR 0")
    if(h.data(1)==["3d" "x" "y" "z" "color"])
        TCL_EvalStr("set flagCOLOR 1")
        d="["+strcat(string(size(h.data.color)),"x")+" integer array]"
        TCL_SetVar("curdata_color",d);
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Plot3d.tcl")
endfunction


function ged_fac3d(h)
    global ged_handle; ged_handle=h

    //  if (h.clip_box==[])
    //    TCL_SetVar("old_Xclipbox","")
    //    TCL_SetVar("old_Yclipbox","")
    //    TCL_SetVar("old_Wclipbox","")
    //    TCL_SetVar("old_Hclipbox","")
    //    TCL_SetVar("Xclipbox","")
    //    TCL_SetVar("Yclipbox","")
    //    TCL_SetVar("Wclipbox","")
    //    TCL_SetVar("Hclipbox","")
    //   else
    //    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    //    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    //  end
    //  TCL_SetVar("curclipstate",h.clip_state);

    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curcolormode",string(h.color_mode))
    TCL_SetVar("colorflagToggle",string(h.color_flag))
    TCL_SetVar("curforeground",string(h.foreground))
    TCL_SetVar("curhiddencolor",string(h.hiddencolor))
    TCL_SetVar("curthick",string(h.thickness))

    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))
    TCL_SetVar("curlinemode",h.surface_mode)


    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.z)),"x")+" double array]"
    TCL_SetVar("curdata_z",d);

    TCL_EvalStr("set flagCOLOR 0")
    if(h.data(1)==["3d" "x" "y" "z" "color"])
        TCL_EvalStr("set flagCOLOR 1")
        d="["+strcat(string(size(h.data.color)),"x")+" integer array]"
        TCL_SetVar("curdata_color",d);
    end


    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Fac3d.tcl")
endfunction


function ged_text(h)
    global ged_handle; ged_handle=h
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curfontforeground",string(h.font_foreground))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curfontangle",string(h.font_angle))
    TCL_SetVar("curtextboxmode",h.text_box_mode)
    TCL_SetVar("curtext",h.text)
    TCL_SetVar("curforeground",string(h.foreground))
    TCL_SetVar("curbackground",string(h.background))
    TCL_SetVar("curboxmode",h.box)
    TCL_SetVar("curlinemode",h.line_mode);
    TCL_SetVar("curfillmode",h.fill_mode);
    //TCL_SetVar("curPosition",h.data) ;

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    if size(h.text,"*") == 1 then
        txt =""""+ h.text +"""" ; // one double quote at each side
    else
        txt = "["+strcat(string(size(h.text)),"x")+" string array]"
    end
    TCL_SetVar("curtext", txt );
    TCL_SetVar( "textBoxWidth", string( h.text_box(1) ) ) ;
    TCL_SetVar("textBoxHeight", string( h.text_box(2) ) ) ;
    TCL_SetVar("curAlignment",string(h.alignment) ) ;

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Text.tcl")
endfunction


function ged_legend(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curforeground",string(h.foreground))
    ged_fontarray = ["Monospaced" "Symbol" "Serif",..
    "Serif Italic" "Serif Bold" "Serif Bold Italic" ,..
    "SansSerif"  "SansSerif Italic" "SansSerif Bold",..
    "SansSerif Bold Italic"];
    TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
    TCL_SetVar("curfontsize",string(h.font_size))
    TCL_SetVar("curtext",h.text)

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Legend.tcl")
endfunction


function ged_arc(h)
    global ged_handle; ged_handle=h

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    TCL_SetVar("nbcol",string(size(h.data,2)))
    for i=1:size(h.data,2)
        val= "arcVAL("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(h.data(2)));
    end
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curfillmode",h.fill_mode)
    TCL_SetVar("curthick",string(h.thickness))

    ax=getparaxe(h);
    // Arc data
    select ax.view
    case "2d"
        drawlater();
        ax.view="3d"  //strange behavior in 3D... seems to be bugged!!
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
        TCL_SetVar("A1val",string(h.data(6)))
        TCL_SetVar("A2val",string(h.data(7)))
        ax.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("Xval",string(h.data(1)))
        TCL_SetVar("Yval",string(h.data(2)))
        TCL_SetVar("Zval",string(h.data(3)))
        TCL_SetVar("Wval",string(h.data(4)))
        TCL_SetVar("Hval",string(h.data(5)))
        TCL_SetVar("A1val",string(h.data(6)))
        TCL_SetVar("A2val",string(h.data(7)))
    end
    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Arc.tcl")
endfunction


function ged_segs(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curarrowsize",string(h.arrow_size))
    TCL_SetVar("curthick",string(h.thickness))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))

    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))
    TCL_SetVar("curlinemode",h.line_mode)

    TCL_SetVar("nbrow",string(size(h.data,1)))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    TCL_SetVar("nbcolsegscolor",string(size(h.segs_color,2)))
    for i=1:size(h.segs_color,2)
        val= "segscolorVAL("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(h.segs_color(i)));
    end

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Segs.tcl")
endfunction


function ged_champ(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curcolored",h.colored)
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curarrowsize",string(h.arrow_size))
    TCL_SetVar("curthick",string(h.thickness))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))

    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.fx)),"x")+" double array]"
    TCL_SetVar("curdata_fx",d);
    d="["+strcat(string(size(h.data.fy)),"x")+" double array]"
    TCL_SetVar("curdata_fy",d);

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Champ.tcl")
endfunction

function ged_fec(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("zbmin",string(h.z_bounds(1)))
    TCL_SetVar("zbmax",string(h.z_bounds(2)))
    //  TCL_SetVar("nbrow",string(size(h.data,1)))
    //  TCL_SetVar("nbcol",string(size(h.data,2)))
    //  TCL_SetVar("nbrowTri",string(size(h.triangles,1)))
    //  TCL_SetVar("nbcolTri",string(size(h.triangles,2)))

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata_data",d);
    d="["+strcat(string(size(h.triangles)),"x")+" double array]"
    TCL_SetVar("curdata_triangles",d);

    //  if (h.clip_box==[])
    //    TCL_SetVar("old_Xclipbox","")
    //    TCL_SetVar("old_Yclipbox","")
    //    TCL_SetVar("old_Wclipbox","")
    //    TCL_SetVar("old_Hclipbox","")
    //    TCL_SetVar("Xclipbox","")
    //    TCL_SetVar("Yclipbox","")
    //    TCL_SetVar("Wclipbox","")
    //    TCL_SetVar("Hclipbox","")
    //   else
    //    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    //    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    //    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    //    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    //    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    //  end
    //  TCL_SetVar("curclipstate",h.clip_state);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Fec.tcl")
endfunction


function ged_grayplot(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curdatamapping",h.data_mapping)


    d="["+strcat(string(size(h.data.x)),"x")+" double array]"
    TCL_SetVar("curdata_x",d);
    d="["+strcat(string(size(h.data.y)),"x")+" double array]"
    TCL_SetVar("curdata_y",d);
    d="["+strcat(string(size(h.data.z)),"x")+" double array]"
    TCL_SetVar("curdata_z",d);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Grayplot.tcl")

endfunction

function ged_matplot(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)

    d="["+strcat(string(size(h.data)),"x")+" double array]"
    TCL_SetVar("curdata",d);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Matplot.tcl")

endfunction


function ged_axis(h)
    global ged_handle; ged_handle=h
    TCL_SetVar("curvis",h.visible)
    TCL_SetVar("curseg",h.tics_segment)
    TCL_SetVar("curcolor",string(h.tics_color))
    TCL_SetVar("curticsstyle",h.tics_style)
    TCL_SetVar("curfontcolor",string(h.labels_font_color))
    TCL_SetVar("curfontsize",string(h.labels_font_size))
    TCL_SetVar("nbcolX",string(size(h.xtics_coord,2)))
    TCL_SetVar("nbcolY",string(size(h.ytics_coord,2)))
    TCL_SetVar("xticscoord",sci2exp(h.xtics_coord,0))
    TCL_SetVar("yticscoord",sci2exp(h.ytics_coord,0))
    TCL_SetVar("cursubtics",string(h.sub_tics))
    TCL_SetVar("curticslabel",sci2exp(h.tics_labels,0))
    TCL_SetVar("curticsdir",string(h.tics_direction))

    if (h.clip_box==[])
        TCL_SetVar("old_Xclipbox","")
        TCL_SetVar("old_Yclipbox","")
        TCL_SetVar("old_Wclipbox","")
        TCL_SetVar("old_Hclipbox","")
        TCL_SetVar("Xclipbox","")
        TCL_SetVar("Yclipbox","")
        TCL_SetVar("Wclipbox","")
        TCL_SetVar("Hclipbox","")
    else
        TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
        TCL_SetVar("Xclipbox",string(h.clip_box(1)))
        TCL_SetVar("Yclipbox",string(h.clip_box(2)))
        TCL_SetVar("Wclipbox",string(h.clip_box(3)))
        TCL_SetVar("Hclipbox",string(h.clip_box(4)))
    end
    TCL_SetVar("curclipstate",h.clip_state);

    TCL_EvalFile(SCI+"/modules/graphics/tcl/ged/Axis.tcl")
endfunction


// compute the square of distance between a point and the ellipse
// in 2D included in an axis aligned rectangle whose upper left
// corner is upperLeft and its wifth and heigth is defined.
function [dist] = dist2Ellipse( point, upperLeft, width, heigth )
    width2  = width  / 2. ;
    heigth2 = heigth / 2. ;
    centerC = [ upperLeft(1) + width2, upperLeft(2) - heigth2 ] ; // center of the ellipse

    // clicked point in the circle frame
    pointC  = [ (point(1) - centerC(1)) / width2, (point(2) - centerC(2)) / heigth2 ] ;

    // get the vector between the point and the closest on the circle
    diffclose = ( 1 - 1 / norm( pointC ) ) * pointC ;
    //closest = pointC / sqrt( pointC(1) * pointC(1) + pointC(2) * pointC(2) ) ;

    // get the difference between the two
    //ffclose = pointC - closest ;

    // bring it back to the current frame value
    diffclose(1) = diffclose(1) * width2  ;
    diffclose(2) = diffclose(2) * heigth2 ;

    // get the distance with the closest point
    dist = norm( diffclose ) ;

endfunction

function [ini,typs]=build_args(labels)
    n=size(labels,"*")
    ini=[]
    typs=list()
    for k=1:n
        typ=type(h(labels(k)))
        execstr(["if typ==10 then"
        "   w=h."+labels(k)
        "else"
        "   w=sci2exp(h."+labels(k)+",0)"
        "end"])
        ini=[ini;w]
        if typ==10 then
            typs($+1)="str";typs($+1)=-1
        else
            typs($+1)="vec";typs($+1)=1
        end
    end
endfunction

function GetSetValue(h)
    n=size(labels,"*")
    lhs="[ok,"+strcat("x"+string(1:n),",")+"]"
    execstr(lhs+"=getvalue(t,labels,typs,ini)")
    if ok then
        for k=1:n
            execstr("if x"+string(k)+"<>h."+labels(k)+" then h."+..
            labels(k)+"=x"+string(k)+",end")
        end
    end
endfunction

function [haxe] = getparaxe( h )

    htmp = h;
    haxe = [];
    while htmp.type<>"Axes" do
        htmp = htmp.parent
    end

    haxe = htmp;
endfunction



// Is called by ged_axes

function LoadTicks2TCL(h)
    global ged_handle;ged_handle=h;

    TCL_SetVar("Xaxes_visibleToggle",h.axes_visible(1))
    TCL_SetVar("Yaxes_visibleToggle",h.axes_visible(2))
    TCL_SetVar("Zaxes_visibleToggle",h.axes_visible(3))

    TCL_SetVar("SubticksEntryX",string(h.sub_ticks(1)))
    TCL_GetVar("SubticksEntryX")

    // disp("h.sub_ticks(1) =")
    // disp(h.sub_ticks(1));

    TCL_SetVar("SubticksEntryY",string(h.sub_ticks(2)))

    select h.view
    case "2d"
        drawlater(); // postpon the drawings due to switching from 2d to 3d mode (for example)
        // in order to know the complete data set (z data for axes...)
        h.view="3d"

        TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
        h.view="2d"
        drawnow();
    case "3d"
        TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
    end

    TCL_SetVar("XautoticksToggle",h.auto_ticks(1))
    TCL_SetVar("YautoticksToggle",h.auto_ticks(2))
    TCL_SetVar("ZautoticksToggle",h.auto_ticks(3))

    //ticks value: X axis
    ticks = h.x_ticks;
    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_x",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_X("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        val= "LABELS_X("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

    //ticks value: Y axis
    ticks = h.y_ticks;
    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_y",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_Y("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        val= "LABELS_Y("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

    //ticks value: Z axis
    ticks = h.z_ticks;
    sizeticks = size(ticks.locations,1);
    TCL_SetVar("nbticks_z",string(sizeticks));
    for i=1:sizeticks
        val= "LOCATIONS_Z("+string(i)+")";
        TCL_EvalStr("set "+val+" "+string(ticks.locations(i)));
        val= "LABELS_Z("+string(i)+")";
        TCL_EvalStr("set "+val+" {"+ticks.labels(i)+"}");
    end

endfunction


function ged_delete_entity()
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    h=ged_getobject([xc,yc])
    if h<>[] then delete(h),end
endfunction

function ged_move_entity()
    [btn,xc,yc]=xclick()
    pos=[xc,yc]
    [xc,yc]=xchange(xc,yc,"f2i")
    [r,ax]=ged_getobject([xc,yc])
    cur_ax=gca(),sca(ax)
    [xc,yc]=xchange(xc,yc,"i2f");pos=[xc,yc]
    if r==[] return,end
    drawlater();
    rep(3)=-1
    select r.type
    case "Rectangle" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Segs" then //Segment
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data=r.data+ones(2,1)*(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Polyline" then //Polyline
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(:,1:2)=r.data(:,1:2)+ones(r.data(:,1))*(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Arc" then //Circle
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Text" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Label" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.position= r.position+(rep(1:2)-pos)
            r.auto_position = "off"
            pos=rep(1:2)
            drawnow();
        end

    end
    sca(cur_ax)
endfunction

function ged_copy_entity()
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    r=ged_getobject([xc,yc])
    if r==[] return,end
    twinkle(r,1);
    save(TMPDIR+"/G_Clipboard",r)
    //make the axes containning the clicked point the current one
    sca(ged_select_axes(xc,yc))
endfunction


function ged_paste_entity()

    // check the file
    [info,err] = fileinfo(TMPDIR + "/G_Clipboard" ) ;

    if err <> 0 then
        return ;
    end

    // create the saved object
    load(TMPDIR+"/G_Clipboard") ;

    //  a=gca();b=a.data_bounds;
    //  move(r,[-1 1]*a.data_bounds/20)
endfunction

function axes =  ged_select_axes(x,y)
    // x and y are pixel coord.
    f=gcf()
    nb_axes = size(f.children,"*") // for now Iconsider that children of a figure are of type Axes
    axes_size = f.axes_size // given in pixels
    axes_size = [axes_size axes_size];

    for i=1:nb_axes
        axes = f.children(i);
        cur_axes_bounds = axes.axes_bounds;
        rect = cur_axes_bounds.*axes_size; // rectangle in pixels (margins inside)

        rect(3) = rect(3) + rect(1);
        rect(4) = rect(4) + rect(2);
        if (x>rect(1) & x<rect(3) & y>rect(2) & y<rect(4)) then
            return
        end
    end
    axes=[]
endfunction
