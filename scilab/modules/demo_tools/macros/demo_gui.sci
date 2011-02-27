//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function demo_gui()
	
	// http://bugzilla.scilab.org/show_bug.cgi?id=6973#c3
	tmp=gdf();
	tmp.figure_position=[0 0]
	// -------------------------
	
	global demolist // Demos list is defined in scilab.start
	
	global margin_x;
	global margin_y;
	
	global frame_w;
	global frame_h;
	
	global button_w;
	global button_h;
	
	// Parameters
	// =========================================================================
	
	frame_w      = 170;  // Frame width
	frame_h      = 350;  // Frame height
	
	margin_x     = 10;   // Horizontal margin between each elements 
	margin_y     = 10;   // Vertical margin between each elements
	
	button_w     = 0;    // Button width
	button_h     = 0;    // Button height
	
	defaultfont  = "arial"; // Default Font
	
	frame_number = 1;    // Frame number
	
	// Figure creation
	// ========================================================================= 
	
	axes_w       = (frame_number+1)*margin_x + frame_number*frame_w; // axes width
	axes_h       = 3*margin_y + frame_h + button_h;                  // axes height
	
	demo_fig = figure(100000);
	
	// Remove Scilab graphics menus & toolbar
	
	delmenu(demo_fig.figure_id, gettext("&File"));
	delmenu(demo_fig.figure_id, gettext("&Tools"));
	delmenu(demo_fig.figure_id, gettext("&Edit"));
	delmenu(demo_fig.figure_id, gettext("&?"));
	toolbar(demo_fig.figure_id, "off");
	demo_fig.visible = "off"; // to be sure that no plot can appear in the plot
	
	// Add the new one
	
	h = uimenu( "parent", demo_fig,          ..
	            "label" , gettext("File"));
	
	uimenu( "parent"    , h,                 ..
	        "label"     , gettext("Close"),  ..
	        "callback"  , "demo_fig=get_figure_handle(100000);delete(demo_fig);", ..
	        "tag"       , "close_menu");
	
	// Background, Resize, title ...
	
	demo_fig.background      = -2;
	demo_fig.axes_size       = [axes_w axes_h];
	demo_fig.figure_name     = gettext("Demos");
	
	// Frame creation
	
	create_frame(demo_fig,1,"Demos",demolist);
	
	demo_fig.axes_size       = [axes_w axes_h];
	
	// pushbutton creation
	
// 	my_button = uicontrol( ...
// 		"parent"              , demo_fig,...
// 		"relief"              , "groove",...
// 		"style"               , "pushbutton",...
// 		"string"              , gettext("View Code"),...
// 		"units"               , "pixels",...
//  	"position"            , [ floor(axes_w/2 - button_w/2) margin_y button_w button_h ], ...
// 		"fontunits"           , "points",...
// 		"fontsize"            , 12,...
// 		"fontweight"          , "bold", ...
// 		"horizontalalignment" , "center", ...
// 		"visible"             , "on", ...
// 		"tag"                 , "button_view_code" );
	
endfunction

//============================================================================

function create_frame(my_fig_handle,fr_position,fr_title,fr_items)
	
	// my_fig_handle : Handle de la figure englobante
	// fr_position   : position de la frame à créer
	// fr_title      : titre de la frame à créer
	// fr_items      : liste des items de la listbox

	// Parameters
	// ------------------------------------------------------------------
	
	frame_w      = 170;     // Frame width
	frame_h      = 350;     // Frame height
	
	margin_x     = 10;      // Horizontal margin between each elements
	margin_y     = 10;      // Vertical margin between each elements
	
	button_w     = 0;       // Button width
	button_h     = 0;       // Button height
	
	defaultfont  = "arial"; // Default Font
	
	// ------------------------------------------------------------------
	
	logoCol=2			// Logo moved on column#2 (shorter list)
	warning("off")
	
	// if no item, no display
	if fr_items == []  then
		my_visible = "off";
	else
		my_visible = "on";
	end
	
	axes_h = my_fig_handle.axes_size(2);
	
	
	if fr_position <> logoCol then	
		this_frame_w     = frame_w;
		this_frame_h     = frame_h;
		this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
		this_frame_pos_y = axes_h-(margin_y+frame_h);
	else
		this_frame_w     = frame_w;
		this_frame_h     = frame_h - 75 ;		// Hauteur du logo.  SG: 150=>75
		this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
		this_frame_pos_y = axes_h-(margin_y+frame_h)+75;	// SG: 150 => 75
	end
	
	// frame
	uicontrol( ...
		"parent"              , my_fig_handle,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x this_frame_pos_y this_frame_w this_frame_h ],...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"visible"             , my_visible, ...
		"tag"                 , "frame_"+string(fr_position));
	
	// Frame title
	uicontrol( ...
		"parent"              , my_fig_handle,...
		"style"               , "text",...
		"string"              , " "+fr_title,...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x+10 axes_h-(margin_y)-9 this_frame_w-20 18 ],...
		"fontunits"           , "points",...
		"fontsize"            , 12,...
		"fontWeight"          , "bold",...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"visible"             , my_visible, ...
		"tag"                 , "title_frame_"+string(fr_position));
		
	// List Box

        callback= "script_path = demo_gui_update(); "+.. // full path
            "path=fileparts(script_path); "+..	   // path without the filename
            "tmp=findobj(""tag"",""demoViewCodeMenu""); "+.. // It is up to the 
            "if tmp~=[], delete(tmp); end; "+..    // dev to allow code viewing
            "isDemo=(grep(script_path,""dem.gateway.sce"") ~= 1); "+..
            "if ~isDemo, "+.. // Category selected 
            "  VCpath = script_path; "+.. 
            "else, "+..       // Demo selected
            "  tmp = mgetl(script_path);"+..  // Reading the code to parse it
            "  tmp = grep(tmp,""uicontrol("");"+.. // Raw parsing for uicontrol(
            "  if tmp~=[], w0 = 100010, else, w0 = 100001; end;"+..
            "  if or(winsid()==w0),"+..            // Some demo developpers 
            "    tmp  = get_figure_handle(w0); "+..   //  forget to clean or 
            "    tmp2 = msprintf(""Figure n°%d"",w0);"+..
            "    tmp.figure_name = tmp2;"+..  // to update the figure's title
            "  end; "+..
            ""+.. // Determining the true target file, for demo_ViewCode:
            ""+.. // If it is not the script script_path itself, it must be
            ""+.. // indicated in it with a trueFile="trueScriptName" 
            ""+.. // litteral instruction, alone on an uncommented line.
            ""+.. // "trueFile" works as a keyword. The filename must be 
            ""+.. // indicated between double quotes, with possibly a relative
            ""+.. // path starting from the script_path's one. If it is in the
            ""+.. // same directory, no relative path must be indicated.
            "  tmp = mgetl(script_path);"+..  // Reading the code, to parse it
            "  tmp2 = grep(tmp,""trueFile"");"+.. // Raw parsing for "trueFile":
            "  tmp = tmp(tmp2); "+..
            "  if tmp~=[] & part(tmp(1),1:2)~=""//"", "+.. 
            "    tmp = tokens(tmp(1),"""""""");"+..
            "    VCpath = path+tmp(2);"+..   // for demo_viewCode
            "  else VCpath = script_path; "+.. // Default
            "  end;"+..
            "  demo_viewCode(VCpath);"+..// Comment to release mandatory display of the ViewCode Menu
            "  scf(w0);"+.. // Default targetted figure. Demo dev. is free 
            "  clf();"+.. // to redirect in the script. Not really disturbing 
            "  show_window(w0);"+.. // if no figure is needed in a specific demo
            "end; "+ ..
            "if getos()==""Windows"", "+ .. // NEW: Display of the script's path
            "  VCpath = getlongpathname(VCpath); "+ .. // in the status bar
            "end; "+ ..
            "if ~isDemo, VCpath = fileparts(VCpath); end; "+ ..
            "fh = get_figure_handle(100000);"+ ..
            "fh.info_message = VCpath; "+ .. 
            "listOfWins = winsid();"+ ..
            "exec(script_path,-1);";

	uicontrol( ...
		"parent"              , my_fig_handle,...
		"style"               , "listbox",...
		"string"              , " "+strcat(fr_items(:,1),"| "),...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x+5 this_frame_pos_y+5 this_frame_w-12 this_frame_h-14-10 ],...
		"fontunits"           , "points",...
		"fontsize"            , 12,...
		"horizontalalignment" , "left", ...
		"BackgroundColor"     , [255/255 , 255/255 , 255/255 ], ...
		"callback"            , callback,...
		"visible"             , my_visible, ...
		"user_data"           , fr_items, ...
		"tag"                 , "listbox_"+string(fr_position));
	
	// Logo Scilab
	// ------------------------------------------------------------------
	
	if fr_position == logoCol then
	
		str = "<html>"+..
					"<img src=""file:///"+SCI+..
					"/modules/demo_tools/images/logo_scilab.png"" height=""75"" width=""75""/>"+..
					"</html>";						// SG: height and width added
		
		my_text = uicontrol( ...
			"parent"              , my_fig_handle,...
			"style"               , "text",...
			"string"              , str,...
			"units"               , "pixels",...
			"position"            , [ (margin_x+(fr_position-1)*(margin_x+frame_w)) axes_h-(margin_y+frame_h) frame_w 80],...
			"background"          , [1 1 1], ...
			"tag"                 , "my_text", ...
			"horizontalalignment" , "center", ...
			"verticalalignment"   , "middle" ...
		);
		
	end
	
	
endfunction

//============================================================================

function script_path = demo_gui_update()

	my_counter = 0;
	
	global subdemolist;
	global frame_number;
	
	my_selframe     = get(gcbo,"tag");
	
	// Suppression d'une figure précédemment dessiné, si figure il y a ...
//	fig_to_del = get_figure_handle(100001);
//	if fig_to_del <> [] then
//		delete(fig_to_del);
//	end
// Commented according to // http://bugzilla.scilab.org/show_bug.cgi?id=6973#c3
	
	// Handle de la figure
	demo_fig        = gcbo.parent;

	// Frame sur laquelle on a cliqué
	my_selframe_num = msscanf(my_selframe,"listbox_%d");
	
	// Récupération de la liste des démos
	my_index = get(findobj("tag",my_selframe), "value"    );
	my_data  = get(findobj("tag",my_selframe), "user_data");
	
	script_path = my_data(my_index(1,1),2);
	
	if grep(script_path,"dem.gateway.sce") == 1 then
		
		// On est dans le cas ou une nouvelle frame va être affichée
		
		// Mise à jour du nombre de frame
		frame_number    = my_selframe_num+1;
		resize_gui(demo_fig,frame_number);
		previous_demolist = subdemolist;
		
		mode(-1);
		exec(script_path,-1);
		create_frame(demo_fig,my_selframe_num+1,my_data(my_index(1,1),1),subdemolist);
		subdemolist = previous_demolist;
		
	else
		// Mise à jour du nombre de frame
		frame_number    = my_selframe_num;
		resize_gui(demo_fig,frame_number);
	end
	
endfunction

//============================================================================

function resize_gui(my_fig_handle,frame_number)
	
	// Parameters
	// ------------------------------------------------------------------
	
	frame_w      = 170;     // Frame width
	frame_h      = 350;     // Frame height
	
	margin_x     = 10;      // Horizontal margin between each elements
	margin_y     = 10;      // Vertical margin between each elements
	
	button_w     = 0;       // Button width
	button_h     = 0;       // Button height
	
	defaultfont  = "arial"; // Default Font
	
	// ------------------------------------------------------------------
	
	axes_w = (frame_number+1)*margin_x + frame_number*frame_w; // axes width
	
	my_fig_handle.axes_size(1) = axes_w;
	
	for i=(frame_number+1):10
		my_frame               = findobj("tag", "frame_"      +string(i));
		if my_frame <> [] then
			delete_frame(my_fig_handle,i);
		else
			break
		end
	end
	
	// update_button_position(my_fig_handle,axes_w);
	
endfunction

//============================================================================

function update_button_position(my_fig_handle,axes_w)
	
	// Update button position
	
	global margin_y;
	global button_w;
	global button_h;
	
	my_button = findobj("tag", "button_view_code");
	my_button.position = [ floor((axes_w/2) - (button_w/2)) margin_y button_w button_h ];
	
endfunction

//============================================================================

function delete_frame(my_fig_handle,fr_position)
	
	my_frame         = findobj("tag", "frame_"      +string(fr_position));
	my_frame_title   = findobj("tag", "title_frame_"+string(fr_position));
	my_listbox       = findobj("tag", "listbox_"    +string(fr_position));
	
	delete(my_frame);
	delete(my_frame_title);
	delete(my_listbox);
	
endfunction
