function choose_sphere()
    fin=%t;
    exec(path+'sphere_interface.sci');   
    exec(path+'sphere.sci'); 
    exesphere();
endfunction

function choose_cylinder()
    fin=%t;
    exec(path+'cylinder_interface.sci');
    exec(path+'cylinder.sci');
    execylinder();
endfunction

function choose_blackhole()
    fin=%t;
    exec(path+'blackhole_interface.sci');
    exec(path+'blackhole.sci');
    exeblackhole();
endfunction

function choose_surface()
        
	f = figure("Position",[30 50 300 200],...
               "BackgroundColor",[0.9 0.9 0.9],...
  	            "Unit"    , "pixel",...
  	            "tag"     , "main");
	   
	title1 = uicontrol(f, "Position"  , [25 160 250 30],...
		 "Style"     , "text",...
		 "String"    , "Choose the surface type",...
		 "BackgroundColor",[0.9 0.9 0.9],...
		 "Fontsize"  , 20);
  		
	button1 = uicontrol(f, "Position"  , [25 120 250 25],...
		 "Style"     , "pushbutton",...
		 "String"    , "Sphere",...
		 "Fontsize"  , 18,...
		 "BackgroundColor",[0.9 0.9 0.9]);
		 
  	button2 = uicontrol(f, "Position"  , [25 90 250 25],...
		 "Style"     , "pushbutton",...
		 "String"    , "Cylinder",...
		 "Fontsize"  , 18,...
		 "BackgroundColor",[0.9 0.9 0.9]);
		 
	button3 = uicontrol(f, "Position"  , [25 60 250 25],...
		 "Style"     , "pushbutton",...
		 "String"    , "Black Hole",...
		 "Fontsize"  , 18,...
		 "BackgroundColor",[0.9 0.9 0.9]);
		 
	button4 = uicontrol(f, "Position"  , [120 20 60 20],...
		 "Style"     , "pushbutton",...
		 "String"    , "cancel",...
		 "Fontsize"  , 14,...
		 "BackgroundColor",[0.9 0.9 0.9]);
		 

	 	
	set(button1,"callback"  , "choose_sphere()");
	set(button2,"callback"  , "choose_cylinder()");
	set(button3,"callback"  , "choose_blackhole()");	
	set(button4,"callback"  , "fin=%t"); 

    fin=%f
    while ~fin
      sleep(1)
      if findobj('tag', 'main')==[] then
        return;
      end
    end
    close(f);		 
endfunction
path=get_absolute_file_path('flow.sce');
choose_surface()  

