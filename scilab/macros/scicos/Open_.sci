function Open_()
// Copyright INRIA
//
//** Comments and mods by Simone Mannori
//** 05 Set 2006 : code from the old "Load_.sci"
// 
//** Open a saved diagram 
  
  Cmenu  = []  ;
  Select = []  ;  //** empty
  //** Select_back = [] ; //** empty
  //** %ppt = []; //** used to store last valid click position for "Paste" operation 
  //** Clipboard = []; //** used in Copy Cut and Paste function  
  
  if edited & ~super_block then //** if is edited and is NOT a superblock
    num = x_message(['Diagram has not been saved'],['OK','Go Back'])
    if num==2 then return;end
    if alreadyran then do_terminate(),end  //terminate current
                                           //simulation
    clear(%scicos_solver)  // avoids forcing implicit compilation in
                           // some situations    
    alreadyran = %f ;
  end

  inactive_windows=close_inactive_windows(inactive_windows,super_path)
  
  xselect();
  
  disablemenus(); 
  
  [ok,scs_m,%cpr,edited] = do_load();
  
  if super_block then edited = %t;end
  
  if ok then
    if ~set_cmap(scs_m.props.options('Cmap')) then 
      scs_m.props.options('3D')(1) = %f; //disable 3D block shape 
    end
    
    options = scs_m.props.options ; 
    
    clf() ; 
    
    set_background();
    
    pwindow_set_size();
    
    window_set_size();
    
    //** ----------------- 
    if type(scs_m.props.context)==10 then
      
      //** %now_win=xget('window')
      %now_win = get("current_figure"); 
      
      [%scicos_context,ierr] = script2var(scs_m.props.context,%scicos_context);
      
      //for backward compatibility for scifunc
      if ierr==0 then
	%mm=getfield(1,%scicos_context)
	for %mi=%mm(3:$)
	  ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
	  if ierr<>0 then
	    break
	  end
	end //** of for 
      end
      //end of for backward compatibility for scifunc
      
      if ierr<>0 then
	message(['Error occur when evaluating context:' lasterror()])
      else
	deff('%fonct()',scs_m.props.context)
	%outfun = macrovar(%fonct);
	clear('%fonct')
	//perform eval only if context contains functions which may give
	//different results from one execution to next
	if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then
	  //** disablemenus() ; //** the menu are already disabled 
	  [scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr) ;
	  //** enablemenus()  ;
	end
      end
      
      set("current_figure", %now_win);
    else
      scs_m.props.context = ' '; //** put the context to void 
    end
    //**--------------
    
    drawlater() ;
    
    drawobjs(scs_m) ;
    
    drawnow() ;show_pixmap() ;
    
    if size(%cpr)==0 then
      needcompile = 4 ;
      alreadyran = %f ;
    else
      %state0 = %cpr.state ;
      needcompile = 0 ;
      alreadyran = %f ;
    end
    
  end
  
  enablemenus();
  
endfunction

