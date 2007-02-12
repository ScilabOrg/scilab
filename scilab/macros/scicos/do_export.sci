function [wa,ha] = do_export(scs_m, fname, titleflag) 
//** 
//** default call inside Export_.sci
//** 
//**  do_export(scs_m) ;
//** 
//**  rhs :=  1  
// titleflag:0 or 1 (place or not place title in eps file)
// used only with fname
  
  [lhs, rhs] = argn(0) ; //** verify the presence of tha arguments
  
  if rhs<3 then //** if no "titleflag" is specified 
      titleflag = 0
  end
  
  rect = dig_bound(scs_m); 
  if rect==[] then
    enablemenus();
    return //** empty diagram 
  end
  
  gh_curwin = gh_current_window ; //** get the handle of the current active window
  
  //** this handle is modified iin cause of graphics output 
  gh_winc =   gh_current_window ; //** get the handle of the current active window
  
  winc = gh_winc.figure_id
  gh_axes = gh_winc.children ; //** axes handle
  
  
  num = 1 ; //** the default is Postscript file output 
  //**------------------------------------------------------------------
  if rhs==1 then //** the default "scs_m only" case 

    num = choose(['Postscript file';..
		  'Graphics window'],'How do you want to export?')

    //** Postscript file -> 1
    //** Graphics window -> 2 
    //** [Cancel]        -> 0
    		 	  
    //** no arguments 
    if num==0 then
         return ; //** EXIT point 
    end
    
    if num<>2 then //** Postscript file -> 1 case 
        fname = savefile('*'); //** open a dialog box with the direcory control
    end                        //** get a valid filename and put it in "fname"
    
  end
  //**------------------------------------------------------------------
  
  
  //**------------------------------------------------------------------
  // remove blanks
  //** if Postscript file AND Unix platform then filter out the blanks
  if num<>2 & ~MSDOS then
    fname = stripblanks(fname)
    ff    = str2code(fname); ff(find(ff==40|ff==53))=36; fname=code2str(ff)
    if fname==emptystr() then return;end   
  end
  //**------------------------------------------------------------------
  
  disablemenus()
  //**------------------ Graphics window -------------------------------
  if num==2 then  //**Graphics window -> 2 
    //** xset('window', max(winsid())+1) //** 
    gh_winc = scf( max(winsid()) + 1 ) ; //** crate a brand new windw and get the handle
    winc = gh_winc.figure_id ; 
    
    //** drawobjs(scs_m, gh_winc) ; 
    //** drawnow(); show_pixmap() ; 
    
    gh_axes = gh_winc.children ; //** axes handle
    copy(gh_curwin.children, gh_winc.children) ;
  
  end
  //**-------------------------------------------------------------------
  
  //** winc = gh_winc.figure_id
  //** colmap = xget('colormap')
  colmap = gh_winc.color_map
  
  //**---------- Geometrical correction ----------------------------------------
  
  wa = (rect(3)-rect(1))
  ha = (rect(4)-rect(2))
  
  if ~exists('%scicos_lr_margin') then %scicos_lr_margin=.1;end
  if ~exists('%scicos_ud_margin') then %scicos_ud_margin=.1;end
  
  rect(1)=rect(1)-wa*%scicos_lr_margin
  rect(3)=rect(3)+wa*%scicos_lr_margin
  rect(2)=rect(2)-ha*%scicos_ud_margin
  rect(4)=rect(4)+ha*%scicos_ud_margin
  
  wa = (rect(3)-rect(1)) ; //** geometrical correction
  ha = (rect(4)-rect(2)) ; 
  
  options = scs_m.props.options
  
  if num<>2 then //** Postscript file -> 1
    
    //**-------------------------- Postscript -----------------------------------
    //** Geometrical correction --------------------------------------
    set_posfig_dim(wa*%zoom/1.8, ha*%zoom/1.8) //** set the coordinates transformation
                                               //** to be used when exporting
                                               //** a graphic window in postscript.
		                 //** using the computed geometrical correction values 				
    //** xsetech(wrect=[0 0 1 1], frect=rect, arect=[0,0,0,0])
    wrect = [0 0 1 1] ;
    gh_axes.axes_bounds = wrect ;
    
    mrect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion
    gh_axes.data_bounds = mrect ; //** default : data_bounds = [xmin ymin ; xmax ymax ]
    
    gh_axes.margins = [0,0,0,0] ; //**
    
    cmap = options.Cmap; 
    
    //** Color correction ------------------------------------------------
    for k=1:size(cmap,1) 
      [mc,kk] = mini(abs(colmap-ones(size(colmap,1),1)*cmap(k,:))*[1;1;1])
      if mc>.0001 then
	colmap = [colmap; cmap(k,:)]
      end
    end
   //** xset('colormap',colmap)
   gh_winc.color_map = colmap ; //** update the new color map 
   
  else
    //**---------------------- Graphics Windows -------------------------------
    //xset('wdim',wa,ha)
    
    gh_winc.figure_size = [wa ha] ; 
    window_set_size()
    if ~set_cmap(options('Cmap')) then // add colors if required
      options('3D')(1)=%f //disable 3D block shape
    end
  end
  //**-------------------------------------------------------------------------
  
  //**----------- Backgroud handling -----------------------------------------
  if ~exists('%scicos_with_background') then
        %scicos_with_background = 1;
  end
  
  if %scicos_with_background then
        set_background()
  end
  
  //**------------------------------------------------------------------------
  //** Put the title in the ouput window/file 
  if ~(rhs==3 & titleflag==0) then
    
    pat = xget('pattern')
    xset('pattern', default_color(0));
    
    width  = (rect(3)-rect(1))/3;
    height = (rect(4)-rect(2))*%scicos_ud_margin;
    xstringb(rect(1)+width,rect(4)-height,scs_m.props.title(1),width,height, 'fill')
    xset('pattern',pat)
  end
  //**------------------------------------------------------------------------
  
  scs_m.props.title(1) = 'Scilab Graphics of ' + scs_m.props.title(1)
  
  //** --------------------- Output ------------------------------------------
  if num<>2 then //** Postscript file -> 1
    drawnow() ; //** draw the window 
    xs2eps(winc, fname) ; //** create the output postscript file 
    set_posfig_dim(0,0) ; //** reset to zero the coordinates transformation to be used when exporting
                          //** a graphic window in postscript.
  else
                  //** Graphics Window
   scf(gh_winc); //** put the focus in the graphics window 
   drawnow() ; show_pixmap() ;
   scf(gh_curwin); 
  
  end 
    
  enablemenus()

endfunction
