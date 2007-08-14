function [wa,ha] = do_export(scs_m, fname, titleflag) 
//** 
//** default call inside Export_.sci
//** 
//**  do_export(scs_m) ;
//** 
//**  rhs :=  1  
// titleflag:0 or 1 (place or not place title in eps file)
// used only with fname
  
  disablemenus()

  [lhs, rhs] = argn(0) ; //** verify the presence of tha arguments
  
  if rhs<3 then //** if no "titleflag" is specified 
    titleflag = 0
  end
  
  rect = dig_bound(scs_m); 
  if rect==[] then
    enablemenus();return //** empty diagram 
  end
  
  gh_curwin = gh_current_window ; //** get the handle of the current active window
  
  //** this handle is modified in case of graphics output 
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
  end   

  if num==0 then
    enablemenus();return ; //** EXIT point 
  elseif num==1 then //** Postscript file 
    fname = savefile('*'); //**  get a valid filename 
    if ~MSDOS then // remove blanks
      fname = stripblanks(fname)
      ff    = str2code(fname); ff(find(ff==40|ff==53))=36; fname=code2str(ff)
      if fname==emptystr() then enablemenus();return;end   
    end	
    driv=driver();
    driver('Pos');
    xinit(fname);
    gh_axes=gca();
    gh_winc =gcf();
  else //**Graphics window -> 2 
    gh_winc = scf( max(winsid()) + 1 ) ; //** create a brand new window and get the handle
    drawlater()    
    winc = gh_winc.figure_id ; 
    gh_axes = gh_winc.children ; //** axes handle
  end
  
  //copy(gh_curwin.children, gh_axes) ;
  

  //**-------------------------------------------------------------------
  
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
  
  if num == 1 then   //**--------------------------Postscript 
   
    //** Geometrical correction
    gh_axes.axes_bounds = [0 0 1 1] ; // use whole graphic window
    
    mrect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion
    gh_axes.data_bounds = mrect ; //** default : data_bounds = [xmin ymin ; xmax ymax ]    
    gh_axes.margins = [0,0,0,0] ; //** no margins
    gh_winc.figure_size = [wa ha] ; 

  else            //**---------------------- Graphics Windows 
    gh_winc.figure_size = [wa ha] ; 
    window_set_size(gh_winc)
  end
  //**-------------------------------------------------------------------------
  
  //**----------- Background handling -----------------------------------------
  if ~exists('%scicos_with_background') then %scicos_with_background = 1;end
  if ~%scicos_with_background then 
    gh_axes.background=-2;
    gh_winc.background=-2;
  end
  //**------------------------------------------------------------------------
  //** Put the title in the ouput window/file 
  if ~(rhs==3 & titleflag==0) then
    width  = (rect(3)-rect(1))/3;
    height = (rect(4)-rect(2))*%scicos_ud_margin;
    xstringb(rect(1)+width,rect(4)-height,scs_m.props.title(1),width,height,'fill')
  end
  //** --------------------- Output ------------------------------------------
  if num == 1 then //** Postscript file 
//    set_posfig_dim(wa*%zoom/1.8, ha*%zoom/1.8) 

    set_posfig_dim(wa, ha) 
    drawobjs(scs_m,gcf())
    drawnow()
    //ChangeAngledTextFont(gh_axes)
    //draw(gh_axes) ; //** draw the window 
    xend();driver(driv);

    if ~exists('%scicos_landscape') then %scicos_landscape=1;end
    opt=""
    if %scicos_landscape then opt=" -landscape ";end
    
    if MSDOS then
      fname=pathconvert(fname,%f,%t,'w')
      comm=pathconvert(SCI+'\bin\BEpsf',%f,%f,'w')
      rep=unix_g(comm+' '+opt+'""'+fname+'""')
    else
      rep=unix_g(SCI+'/bin/BEpsf '+opt+fname)
    end
    
    if rep<>[] then 
      message(['Problem generating ps file.';'perhaps directory not writable'] )
    end

    //** reset  the postscript export coordinates transformation
    set_posfig_dim(0,0) ; 
		  
  else
    //** Graphics Window
    scf(gh_winc); //** put the focus in the graphics window 
    ChangeAngledTextFont(gh_winc)
    drawnow() ; show_pixmap() ;
    
  end 
  scf(gh_curwin); 
  drawnow() ; show_pixmap() ;
  enablemenus()

endfunction
