//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function demo_gui()

  global demolist; // Demos list is defined in scilab.start
  global demolistlock;
  if isempty(demolist) then
    if isempty(demolistlock) then
      demolistlock = %t;
      // we load scilab demos only when it is required
      modules = getmodules();
      for i=1:size(modules,"*")
        if isfile("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce") then
          exec("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce",-1);
        end
      end
      clear demolistlock;
      clearglobal demolistlock;
    end
  end

  // Figure creation
  // =========================================================================

  demo_fig     = figure(100000);
  demo_fig.tag = "scilab_demo_fig";

  // Parameters
  // =========================================================================

  demo_fig.userdata = struct();
  demo_fig.userdata.frame_w  = 200;   // Frame width
  demo_fig.userdata.frame_h  = 450;   // Frame height

  demo_fig.userdata.margin_x = 15;    // Horizontal margin between each elements
  demo_fig.userdata.margin_y = 15;    // Vertical margin between each elements

  demo_fig.userdata.frame_number = 1;      // Frame number

  demo_fig.userdata.subdemolist = [];


  axes_w     = (demo_fig.userdata.frame_number+1)*demo_fig.userdata.margin_x + ..
               demo_fig.userdata.frame_number*demo_fig.userdata.frame_w; // axes width
  axes_h     = 3*demo_fig.userdata.margin_y + demo_fig.userdata.frame_h; // axes height

  // Remove Scilab graphics menus & toolbar

  delmenu(demo_fig.figure_id, gettext("&File"));
  delmenu(demo_fig.figure_id, gettext("&Tools"));
  delmenu(demo_fig.figure_id, gettext("&Edit"));
  delmenu(demo_fig.figure_id, gettext("&?"));
  toolbar(demo_fig.figure_id, "off");
  demo_fig.visible = "off"; // to be sure that no plot can appear in the plot

  // Add the new one

  h = uimenu( "parent", demo_fig,      ..
              "label" , gettext("File"));

  lst_vars_locals = ["%h_delete";
                     "demo_fig";
                     "get_figure_handle";
                     "subdemolist";
                     "demolistlock";
                     "delete_frame";
                     "resize_gui";
                     "demo_gui_update";
                     "create_frame";
                     "demo_gui"];

  clear_vars_str = strcat("clear " + lst_vars_locals, ";") + ";";

  callback_close_str = "demo_fig=get_figure_handle(100000);delete(demo_fig);";
  callback_close_str = callback_close_str + clear_vars_str + "clearglobal demolistlock;";

  uimenu( "parent"  , h,         ..
          "label"   , gettext("Close"),  ..
          "callback"  , callback_close_str, ..
          "tag"     , "close_menu");

  // Background, Resize, title ...

  demo_fig.background    = -2;
  demo_fig.axes_size     = [axes_w axes_h];
  demo_fig.figure_name   = gettext("Demos");

  // Frame creation

  create_frame(demo_fig,1,"Demos",demolist);

  demo_fig.axes_size     = [axes_w axes_h];

  demo_fig.resizefcn = "resize_demo_gui";

endfunction

function resize_demo_gui()

demo_fig = findobj("tag", "scilab_demo_fig");

margin_x   = demo_fig.userdata.margin_x;    // Horizontal margin between each elements
margin_y   = demo_fig.userdata.margin_y;    // Vertical margin between each elements

axes_w = demo_fig.axes_size(1);
axes_h = demo_fig.axes_size(2);

for fr_position = 1:demo_fig.userdata.frame_number
  curFrame = findobj("Tag", "frame_"+string(fr_position));
  curTitle = findobj("Tag", "title_frame_"+string(fr_position));
  curListbox = findobj("Tag", "listbox_"+string(fr_position));
  scilabLogo = findobj("Tag", "scilab_logo");

  frame_w = (axes_w-(demo_fig.userdata.frame_number+1)*margin_x)/demo_fig.userdata.frame_number;
  frame_h = axes_h-margin_y*2;

  demo_fig.userdata.frame_w = frame_w;
  demo_fig.userdata.frame_h = frame_h;

  if fr_position <> 1 then
    this_frame_w   = frame_w;
    this_frame_h   = frame_h;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h);
  else
    this_frame_w   = frame_w;
    this_frame_h   = frame_h - 150 ;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h)+150;
  end

  if ~isempty(curFrame) then
    set(curFrame, "position", [ this_frame_pos_x this_frame_pos_y this_frame_w this_frame_h ]);
    set(curTitle, "position", [ this_frame_pos_x+10 axes_h-(margin_y)-9 this_frame_w-20 18 ]);
    set(curListbox, "position", [ this_frame_pos_x+5 this_frame_pos_y+5 this_frame_w-12 this_frame_h-14-10 ]);
    set(scilabLogo, "position", [ margin_x axes_h-(margin_y+frame_h) frame_w 150]);
  end

end

endfunction

function create_frame(my_fig_handle,fr_position,fr_title,fr_items)

  // my_fig_handle : Handle de la figure englobante
  // fr_position   : position de la frame � cr�er
  // fr_position   : titre de la frame � cr�er
  // fr_items    : liste des items de la listbox

  // Parameters
  // =========================================================================

  frame_w    = my_fig_handle.userdata.frame_w;   // Frame width
  frame_h    = my_fig_handle.userdata.frame_h;   // Frame height

  margin_x   = my_fig_handle.userdata.margin_x;    // Horizontal margin between each elements
  margin_y   = my_fig_handle.userdata.margin_y;    // Vertical margin between each elements

  // =========================================================================


  // if no item, no display
  if fr_items == []  then
    my_visible = "off";
  else
    my_visible = "on";
  end

  axes_h = my_fig_handle.axes_size(2);


  if fr_position <> 1 then
    this_frame_w   = frame_w;
    this_frame_h   = frame_h;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h);
  else
    this_frame_w   = frame_w;
    this_frame_h   = frame_h - 150 ;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h)+150;
  end

  // frame
  curFrame = findobj("tag", "frame_"+string(fr_position));
  if ~isempty(curFrame) then
      delete_frame(my_fig_handle, fr_position);
  end
  uicontrol( ...
    "parent"        , my_fig_handle,...
    "relief"        , "groove",...
    "style"         , "frame",...
    "units"         , "pixels",...
    "position"      , [ this_frame_pos_x this_frame_pos_y this_frame_w this_frame_h ],...
    "horizontalalignment" , "center", ...
    "background"      , [1 1 1], ...
    "visible"       , my_visible, ...
    "tag"         , "frame_"+string(fr_position));

  // Frame title
  uicontrol( ...
    "parent"        , my_fig_handle,...
    "style"         , "text",...
    "string"        , " "+gettext(fr_title),...
    "units"         , "pixels",...
    "position"      , [ this_frame_pos_x+10 axes_h-(margin_y)-9 this_frame_w-20 18 ],...
    "fontunits"       , "points",...
    "fontsize"      , 12,...
    "fontWeight"      , "bold",...
    "horizontalalignment" , "center", ...
    "background"      , [1 1 1], ...
    "visible"       , my_visible, ...
    "tag"         , "title_frame_"+string(fr_position));

  // List Box
  uicontrol( ...
    "parent"        , my_fig_handle,...
    "style"         , "listbox",...
    "string"        , " "+strcat(fr_items(:,1),"| "),...
    "units"         , "pixels",...
    "position"      , [ this_frame_pos_x+5 this_frame_pos_y+5 this_frame_w-12 this_frame_h-14-10 ],...
    "fontunits"       , "points",...
    "fontsize"      , 12,...
    "horizontalalignment" , "left", ...
    "BackgroundColor"   , [255/255 , 255/255 , 255/255 ], ...
    "callback"      , "script_path = demo_gui_update();exec(script_path,-1);clear script_path;",...
    "visible"       , my_visible, ...
    "user_data"       , fr_items, ...
    "tag"         , "listbox_"+string(fr_position));

  // Logo Scilab
  // =========================================================================

  if fr_position == 1 then

    uicontrol( ...
      "parent"        , my_fig_handle,...
      "style"         , "image",...
      "string"        , SCI+"/modules/demo_tools/images/logo_scilab.png",...
      "units"         , "pixels",...
      "position"      , [ (margin_x+(fr_position-1)*(margin_x+frame_w)) axes_h-(margin_y+frame_h) frame_w 150],...
      "background"      , [1 1 1], ...
      "tag"         , "scilab_logo", ...
      "horizontalalignment" , "center", ...
      "verticalalignment"   , "middle" ...
    );

  end


endfunction

function script_path = demo_gui_update()

  my_counter = 0;

  global subdemolist;
  
  my_selframe   = get(gcbo,"tag");

  // Suppression d'une figure pr�c�demment dessin�e, si figure il y a ...
  all_figs = winsid();
  all_figs = all_figs(all_figs >= 100001); // All Scilab graphic windows opened for demos
  for fig_index = 1:size(all_figs, "*")
    fig_to_del = get_figure_handle(all_figs(fig_index));
    if ~isempty(fig_to_del) then
      delete(fig_to_del);
    end
  end

  // Handle de la figure
  demo_fig    = gcbo.parent;
  
  // Frame sur laquelle on a cliqu�
  my_selframe_num = msscanf(my_selframe,"listbox_%d");

  // R�cup�ration de la liste des d�mos
  my_index = get(findobj("tag",my_selframe), "value"  );
  my_data  = get(findobj("tag",my_selframe), "user_data");

  script_path = my_data(my_index(1,1),2);

  if grep(script_path,"dem.gateway.sce") == 1 then

    // On est dans le cas ou une nouvelle frame va �tre affich�e

    // Mise � jour du nombre de frame
    demo_fig.userdata.frame_number  = my_selframe_num+1;
    resize_gui(demo_fig, demo_fig.userdata.frame_number);
    previous_demolist    = demo_fig.userdata.subdemolist;

    mode(-1);
    exec(script_path,-1); // This script erases subdemolist variable if needed

    // Create a temporary variable for userdata
    // because mixing handles and structs can lead to problems
    ud = demo_fig.userdata;
    ud.subdemolist = subdemolist;
    demo_fig.userdata = ud;
    clearglobal subdemolist

    create_frame(demo_fig,my_selframe_num+1,my_data(my_index(1,1),1), demo_fig.userdata.subdemolist);
    demo_fig.userdata.subdemolist = previous_demolist;

  else
    // Mise � jour du nombre de frame
    demo_fig.userdata.frame_number  = my_selframe_num;
    resize_gui(demo_fig, demo_fig.userdata.frame_number);
  end

endfunction

function resize_gui(my_fig_handle,frame_number)

  // Parameters
  // =========================================================================

  frame_w    = my_fig_handle.userdata.frame_w;   // Frame width
  frame_h    = my_fig_handle.userdata.frame_h;   // Frame height

  margin_x   = my_fig_handle.userdata.margin_x;    // Horizontal margin between each elements
  margin_y   = my_fig_handle.userdata.margin_y;    // Vertical margin between each elements

  // =========================================================================

  axes_w           = (frame_number+1)*margin_x + frame_number*frame_w; // axes width
  my_fig_handle.axes_size(1) = axes_w;

  for i=(frame_number+1):10

    my_frame         = findobj("tag", "frame_"    +string(i));
    my_frame_title     = findobj("tag", "title_frame_"+string(i));
    my_listbox       = findobj("tag", "listbox_"  +string(i));

    if my_frame <> [] then
      delete_frame(my_fig_handle,i);
    end

  end

endfunction

function delete_frame(my_fig_handle,fr_position)

  my_frame         = findobj("tag", "frame_"    +string(fr_position));
  my_frame_title     = findobj("tag", "title_frame_"+string(fr_position));
  my_listbox       = findobj("tag", "listbox_"  +string(fr_position));

  delete(my_frame);
  delete(my_frame_title);
  delete(my_listbox);

endfunction
