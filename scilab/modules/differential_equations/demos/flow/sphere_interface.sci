//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//

function exesphere()
    // initialize the graphics and controls

    // initial point
    g_r = 0.7;//radius
    g_t = 250;//theta

    //initial speed
    g_V    = 1; //speed
    g_Vdir = 180; //speed orientation



    // Figure parameters
    // =========================================================================

    surface_frame_w = 600;     // surface frame width
    surface_frame_h = 600;     // surface frame height

    control_frame_w = 600;
    control_frame_h = 200;

    margin_x     = 15;      // Horizontal margin between each elements
    margin_y     = 15;      // Vertical margin between each elements

    axes_figure_w = 2*margin_x + max(surface_frame_w,control_frame_w);
    axes_figure_h = 3*margin_y + surface_frame_h + control_frame_h;

    // Figure creation
    // =========================================================================

    my_figure_handle = createMainWindow([axes_figure_w axes_figure_h], _("Sphere"), "main_figure");

//    my_figure_handle.background      = -2;
//    my_figure_handle.figure_position = [0 0];
//    my_figure_handle.axes_size       = [axes_figure_w axes_figure_h];
//    my_figure_handle.figure_name     = gettext("Sphere");

    //reserve The upper part of the graphic windows for the 3d view of the scene
//    curAxe=gca();
//    curAxe.axes_bounds = [0,0,1,3/4];

    //Simulation parameters
    speed = tlist(["sim_param", "name","unit","range","default","tag"],...
    "speed", ...
    "m/s", ...
    [0 100],...
    1,...
    "speed");

    direction = tlist(["sim_param", "name","unit","range","default","tag"],...
    "direction", ...
    "deg", ...
    [0 100],...
    50,...
    "dir");

    r = tlist(["sim_param", "name","unit","range","default","tag"],...
    "r", ...
    "m", ...
    [0 100],...
    0,...
    "r");

    theta = tlist(["sim_param", "name","unit","range","default","tag"],...
    "theta", ...
    "deg", ...
    [0 100],...
    1,...
    "theta");
    
    //Create the controls in the window bottom
    // =========================================================================   
    param_list = list(direction, theta, speed, r);
    [plot_frame, param_frame, button_frame] = create_gui(my_figure_handle);
    create_param_zone(param_frame, param_list);
    create_buttons(button_frame);


    draw_sphere();
    draw_initial_point(g_r,g_t,g_V,g_Vdir,%T);



//    sphere_create_gui()

    //Set control and graphics initial values
    // =========================================================================
    change_r(g_r)
    change_theta(g_t)
    change_speed(g_V)
    change_dir(g_Vdir)

my_figure_handle.visible = "on";
endfunction

// draw_sphere
// =============================================================================

function draw_sphere()
    // draw half of a sphere (the flow lines surface )

    function [x,y,z]=sph(alp,tet)
        //parametric definition of a sphere
        x = r*cos(alp).*cos(tet)+orig(1)*ones(tet)
        y = r*cos(alp).*sin(tet)+orig(2)*ones(tet)
        z = r*sin(alp)+orig(3)*ones(tet);
    endfunction

    r    = 1;
    orig = [0 0 0];
    //create surface facets
    [xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,0,40),linspace(0,%pi*2,20));
    // draw facets
    plot3d(xx,yy,zz);

endfunction


// draw_initial_point
// =============================================================================
function draw_initial_point(g_r,g_t,g_V,g_Vdir,create)
    x = g_r*cos(g_t*%pi/180);
    y = g_r*sin(g_t*%pi/180);
    r = 0.05;
    z = -sqrt(1-x^2-y^2)+r;
    dx1 = 0.1*g_V*cos(g_Vdir*%pi/180);
    dy1 = 0.1*g_V*sin(g_Vdir*%pi/180);
    my_figure_handle = get("main_figure");
    my_figure_handle.immediate_drawing = "off";

    if create then
        //create the graphical object used to visualize the trajectory
        xpoly(0,0)
        traj_handle                        =gce();
        traj_handle.thickness              = 2;
        traj_handle.foreground             = 5;
        //create the graphical object used to visualize the initial speed
        xpoly(0,0)
        speed_handle                       =  gce();
        speed_handle.thickness             =  1;
        speed_handle.foreground            =  5;
        speed_handle.polyline_style        =  4;
        speed_handle.arrow_size_factor     =  2;
        //create the graphical objects used to visualize the initial position
        param3d([x-r,x+r],[y,y],[z,z]);
        e1                                 = gce();
        e1.thickness                       = 1;
        e1.foreground                      = 5;
        param3d([x,x],[y-r,y+r],[z,z]);
        e2                                 = gce();
        e2.thickness                       = 1;
        e2.foreground                      = 5;
        param3d([x,x],[y,y],[z-r,z+r]);
        e3                                 = gce();
        e3.thickness                       = 1;
        e3.foreground                      = 5;
        glue([traj_handle speed_handle e1 e2 e3])
    else
        curAxe = gca();
        initial=curAxe.children(1).children
        initial(1).data = [x,y,z-r;x,y,z+r];
        initial(2).data = [x,y-r,z;x,y+r,z];
        initial(3).data = [x-r,y,z;x+r,y,z];
    end
    z1=-sqrt(1-(x+dx1)^2-(y+dy1)^2)+r
    initial(4).data = [x,y,z;x+dx1,y+dy1,z1];

    my_figure_handle.immediate_drawing = "on";
endfunction

// =============================================================================
// The gui controls builder
// =============================================================================

function sphere_create_gui()
    //initial values

    g_t     = 0;
    g_r     = 0;
    g_speed = 0;
    g_Vdir   = 0;

<<<<<<< Updated upstream
    my_figure_handle = scf(100001);
=======
    my_figure_handle = get("main_figure");
    my_figure_handle.figure_size(1) = my_figure_handle.figure_size(1) + 120;
>>>>>>> Stashed changes

    axes_size_figure = my_figure_handle.axes_size;
    axes_figure_w    = axes_size_figure(1);
    axes_figure_h    = axes_size_figure(2);

    frame_x      = 25;
    frame_y      = 200;

    slider_height   = 15;
    slider_width    = 140;
    value_width     = 30;
    unit_width      = 30;
    text_width      = 50;
    y_margin        = 5;
    x_margin        = 10;
    title_width     =  text_width + slider_width + value_width + unit_width +3*x_margin

    x_text         = frame_x
    x_slider        = x_text + text_width + x_margin;
    x_value         = x_slider + slider_width+x_margin;
    x_unit          = x_value  + value_width+x_margin;

    y               = frame_y-40;

    // Title position
    // =========================================================================

    title_r = uicontrol(my_figure_handle                             , ...
    "position"            , [x_text y title_width slider_height+y_margin]   , ...
    "Style"               , "text"                                   , ...
    "String"              , "Starting point"                         , ...
    "BackgroundColor"     , [1 1 1]                                  , ...
    "Fontsize"            , 16                                       , ...
    "horizontalalignment" , "center"                                 , ...
    "tag"                 , "title_r"                            );

    // Slider "r"
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_r = uicontrol(my_figure_handle                              , ...
    "position"          , [x_text y text_width slider_height]    , ...
    "Style"             , "text"                                 , ...
    "String"            , "r"                                    , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "text_r"                           );

    slider_r = uicontrol(my_figure_handle                            , ...
    "position"          , [x_slider y slider_width slider_height], ...
    "Style"             , "slider"                               , ...
    "Value"             , g_r                                 , ...
    "Min"               , 0                                      , ...
    "Max"               , 100                                      , ...
    "callback"          , "change_r()"                           , ...
    "tag"               , "slider_r"                         );

    value_r = uicontrol(my_figure_handle                             , ...
    "position"          , [x_value  y value_width slider_height] , ...
    "Style"             , "text"                                 , ...
    "String"            , string(g_r)                            , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "value_r"                          );

    unite_r = uicontrol(my_figure_handle                             , ...
    "position"          , [x_unit y unit_width slider_height]    , ...
    "Style"             , "text"                                 , ...
    "String"            , "(m)"                                  , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "unite_r"                          );

    // Slider theta
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_theta = uicontrol(my_figure_handle                          , ...
    "position"       , [x_text y text_width slider_height]       , ...
    "Style"          , "text"                                    , ...
    "String"         , "theta"                                   , ...
    "BackgroundColor", [1 1 1]                                   , ...
    "tag"            , "text_theta"                              );

    slider_theta = uicontrol(my_figure_handle                        , ...
    "position"  , [x_slider y slider_width slider_height]        , ...
    "Style"     , "slider"                                       , ...
    "Min"       , 0                                              , ...
    "Max"       , 100                                            , ...
    "Value"     , g_t                                            , ...
    "callback"  , "change_theta()"                                   , ...
    "tag"       , "slider_theta"                                 );

    value_theta = uicontrol(my_figure_handle                         , ...
    "position"        , [x_value  y value_width  slider_height]  , ...
    "Style"           , "text"                                   , ...
    "String"          , string(g_t)                              , ...
    "BackgroundColor" , [1 1 1]                                  , ...
    "tag"             , "value_theta"                            );

    unite_theta = uicontrol(my_figure_handle                         , ...
    "position"        , [x_unit y unit_width slider_height]      , ...
    "Style"           , "text"                                   , ...
    "String"          , "(deg)"                                  , ...
    "BackgroundColor" , [1 1 1]                                  , ...
    "tag"             , "unite_theta"                            );
    //---------------------------------------------------------------------------
    //second column
    x_text          = frame_x + title_width + 3*x_margin
    x_slider        = x_text + text_width + x_margin;
    x_value         = x_slider + slider_width+x_margin;
    x_unit          = x_value  + value_width+x_margin;

    y               = frame_y-40;

    // Title speed
    // =========================================================================

    title_speed = uicontrol(my_figure_handle                             , ...
    "position"            , [x_text y title_width slider_height+y_margin],...
    "Style"               , "text"                                   , ...
    "String"              , "Initial speed"                          , ...
    "BackgroundColor"     , [1 1 1]                                  , ...
    "Fontsize"            , 16                                       , ...
    "horizontalalignment" , "center"                                 , ...
    "tag"                 , "title_speed"                            );

    // Slider speed
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_speed = uicontrol(my_figure_handle                          , ...
    "position"          , [x_text y text_width slider_height]    , ...
    "Style"             , "text"                                 , ...
    "String"            , "speed"                                , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "text_speed"                           );

    slider_speed = uicontrol(my_figure_handle                        , ...
    "position"          , [x_slider y slider_width slider_height], ...
    "Style"             , "slider"                               , ...
    "Value"             , g_speed                                , ...
    "Min"               , 0                                      , ...
    "Max"               , 100                                      , ...
    "callback"          , "change_speed()"                       , ...
    "tag"               , "slider_speed"                         );

    value_speed = uicontrol(my_figure_handle                         , ...
    "position"          , [x_value  y value_width slider_height] , ...
    "Style"             , "text"                                 , ...
    "String"            , string(g_speed)                        , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "value_speed"                          );

    unite_speed = uicontrol(my_figure_handle                         , ...
    "position"          , [x_unit y unit_width slider_height]    , ...
    "Style"             , "text"                                 , ...
    "String"            , "(m/s)"                                  , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "unite_speed"                          );

    // Slider direction
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_dir = uicontrol(my_figure_handle                            , ...
    "position"       , [x_text y text_width slider_height]       , ...
    "Style"          , "text"                                    , ...
    "String"         , "direction"                                   , ...
    "BackgroundColor", [1 1 1]                                   , ...
    "tag"            , "text_dir"                              );

    slider_dir = uicontrol(my_figure_handle                          , ...
    "position"  , [x_slider y slider_width slider_height]        , ...
    "Style"     , "slider"                                       , ...
    "Min"       , 0                                              , ...
    "Max"       , 100                                            , ...
    "Value"     , g_Vdir                                           , ...
    "callback"  , "change_dir()"                                   , ...
    "tag"       , "slider_dir"                                 );

    value_dir = uicontrol(my_figure_handle                           , ...
    "position"        , [x_value  y value_width  slider_height]  , ...
    "Style"           , "text"                                   , ...
    "String"          , string(g_Vdir)                              , ...
    "BackgroundColor" , [1 1 1]                                  , ...
    "tag"             , "value_dir"                            );

    unite_dir = uicontrol(my_figure_handle                           , ...
    "position"        , [x_unit y unit_width slider_height]      , ...
    "Style"           , "text"                                   , ...
    "String"          , "(deg)"                                  , ...
    "BackgroundColor" , [1 1 1]                                  , ...
    "tag"             , "unite_dir"                            );
    // Buttons
    // =========================================================================
    y = y - (slider_height+5*y_margin)
    btn_width=100;
    btn_margin=50;
    btn_height=20;
    x=frame_x+80
    start_button = uicontrol(my_figure_handle                                   , ...
    "Position"             , [x y btn_width btn_height]                 , ...
    "Style"                , "pushbutton"                               , ...
    "String"               , "Start"                                    , ...
    "Fontsize"             , 14                                         , ...
    "BackgroundColor"      , [1 1 1]                                    , ...
    "callback"             , "start_simu()"                             , ...
    "tag"                  , "start_button"                             );

    x=x+btn_width+btn_margin
    stop_button = uicontrol(my_figure_handle                                    , ...
    "Position"               , [x y btn_width btn_height]               , ...
    "Style"                  , "pushbutton"                             , ...
    "String"                 , "Stop"                                   , ...
    "Fontsize"               , 14                                       , ...
    "BackgroundColor"        , [1 1 1]                                  , ...
    "callback"               , "stop_simu"                              , ...
    "tag"                    , "stop_button"                            );

    x=x+btn_width+btn_margin
    clear_button = uicontrol(my_figure_handle                                   , ...
    "Position"               , [x y btn_width btn_height]               , ...
    "Style"                  , "pushbutton"                             , ...
    "String"                 , "Clear"                                  , ...
    "Fontsize"               , 14                                       , ...
    "BackgroundColor"        , [1 1 1]                                  , ...
    "callback"               , "clear_simu"                             , ...
    "tag"                    , "clear_button"                           );

    return
endfunction

// =============================================================================
// The callbacks
// =============================================================================

function change_r(r)
    //r slider callback
    // r is in [0 1]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    slider_r     = get("slider_r");

    value_r       = get("value_r");
    if argn(2)==1 then
        slider_r.Value=(r)*100;
    else
        r=slider_r.Value
        r=r/100
    end
    value_r.String=msprintf("%.3f",r)
    draw_initial_point(slider_r.Value/100,slider_theta.value*360/100,slider_speed.Value*20/100,slider_dir.value*360/100,%F);

endfunction


function change_theta(theta)
    //theta slider callback
    //theta is in [0 360]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    slider_r     = get("slider_r");

    value_theta   = get("value_theta");
    if argn(2)==1 then
        slider_theta.Value=(theta)*100/360;
    else
        theta=slider_theta.Value
        theta=theta*360/100
    end
    value_theta.String=msprintf("%.0f",theta)
    draw_initial_point(slider_r.Value/100,slider_theta.value*360/100,slider_speed.Value*20/100,slider_dir.value*360/100,%F);
endfunction

function change_speed(speed)
    //speed slider callback
    //speed is in [0 20]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");

    value_speed  = get("value_speed");
    if argn(2)==1 then
        slider_speed.Value=(speed)*100/20;
    else
        speed=slider_speed.Value
        speed=speed*20/100
    end
    value_speed.String=msprintf("%.0f",speed)
    draw_initial_point(slider_r.Value/100,slider_theta.value*360/100,slider_speed.Value*20/100,slider_dir.value*360/100,%F);
endfunction

function change_dir(dir)
    //direction slider callback
    //dir is in [0 360]
    slider_r      = get("slider_r");
    slider_theta  = get("slider_theta");
    slider_speed  = get("slider_speed");
    slider_dir    = get("slider_dir");
    value_dir     = get("value_dir");
    if argn(2)==1 then
        slider_dir.Value=(dir)*100/360;
    else
        dir=slider_dir.Value
        dir=dir*360/100
    end
    value_dir.String=msprintf("%.0f",dir)
    draw_initial_point(slider_r.Value/100,slider_theta.value*360/100,slider_speed.Value*20/100,slider_dir.value*360/100,%F);
endfunction

function start_simu()
    //start button callback
    my_figure_handle           = get("main_figure");
    fin                        = my_figure_handle.user_data
    my_figure_handle.user_data = %f

    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    t =            0:0.01:15
    Y = calculate_traj(slider_r.Value/100,slider_theta.value*360/100, ...
    slider_speed.Value*20/100,slider_dir.value*360/100,t)
    x = Y(1,:)
    y = Y(3,:)
    r = 0.1; //bias to have the curve above the surface
    z = -sqrt(1-x.^2-y.^2)+r;
    my_figure_handle = get("main_figure");
    curAxe = gca();
    traj_handle=curAxe.children(1).children(5);
    traj_handle.data=[x(1),y(1),z(1)];
    for k=2:size(x,"*")
        sleep(10)
        if execstr("fin=my_figure_handle.user_data","errcatch")<>0|fin then break,end
        traj_handle.data=[traj_handle.data;[x(k),y(k),z(k)]];
    end
    my_figure_handle.user_data=%t
endfunction

function stop_simu()
    //stop button callback
    my_figure_handle = get("main_figure");
    fin   = %T;
    my_figure_handle.user_data=fin
endfunction

function clear_simu()
    //clear button callback
    my_figure_handle = get("main_figure");
    fin=my_figure_handle.user_data
    if fin then
        my_figure_handle = get("main_figure");
        curAxe = gca();
        traj_handle=curAxe.children(1).children(5);
        traj_handle.data=[0,0,0];
    end
endfunction
