function scs_show(scs_m, win)
// Copyright INRIA
//**
//** 23 Nov 2006: Fist conversion 
//**
  
  disp("scs_show active...")

  //** oldwin = xget('window')
  gh_oldwin = gcf(); //** save the handle of the sel. windows
  
  //** xset('window',win); xbasc()
  gh_curwin = scf(win) ;
  
  if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
    scs_m.props.options('3D')(1)=%f //disable 3D block shape
  end
  
  wsiz = scs_m.props.wpar       ;
  options = scs_m.props.options ;
  
  set_background()              ;

  //** xset('wdim',wsiz(1),wsiz(2))
  gh_curwin.figure_size = [ wsiz(1) wsiz(2) ] ; //** 
  
  rect = dig_bound(scs_m) ; //** compute the diagram size 
  wa = (rect(3)-rect(1))  ;
  ha = (rect(4)-rect(2))  ;
  aa = wa/ha              ;
  rr = wsiz(1)/wsiz(2)    ; 

  if aa<rr then 
    wa2 = wa*rr/aa               ;
    rect(1) = rect(1)-(wa2-wa)/2 ;
    rect(3) = rect(1)+wa2        ;
  else
    ha2 = ha*aa/rr               ;
    rect(2) = rect(2)-(ha2-ha)/2 ; 
    rect(4) = rect(2)+ha2
  end

  dxx = (rect(3)-rect(1))/20 ;
  dyy = (rect(4)-rect(2))/20 ;
  rect(1) = rect(1)-dxx; rect(3) = rect(3)+dxx ;
  rect(2) = rect(2)-dyy; rect(4) = rect(4)+dyy ;
  
  //** xsetech([-1 -1 8 8]/6, rect) ;

     gh_curwin.auto_resize = "off" ; //**
    
     // gh_curwin.axes_size = [width height] ; 
  
     //** axes settings 
     gh_axes = gh_curwin.children ; //** axes handle
     gh_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode"
  
     //** The default margin are [ 0.125 0.125 0.125 0.125 ]
     arect = [0.02,0.02,0.02,0.02] //** margins (default normalized values)
     gh_axes.margins = arect ;     //**
  
     //** 
     mrect = [rect(1) rect(2) ; rect(3) rect(4)] ; //** vector to matrix conversion   
     gh_axes.data_bounds = mrect ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]
  
     //** wrect = [bx , by, 1/ax, 1/ay] ;
     wrect = [-1 -1 8 8] / 6.0 ;  
     gh_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0 , 1,1] = [xmin ymin xmax ymax] 
    
  //** Optional (?) pan control 
  // %XSHIFT = max( (width - r(1) ) / 2, 0) ;
  // %YSHIFT = max( (height- r(2) ) / 2, 0) ;
  //** This correction is really needed BUT uses old graphics primitives :(
  // if ~MSDOS then %YSHIFT = %YSHIFT+30 ; end //** correction for the UNIX system 
  //** Beware ! : I'm forced to use a couple of old graphics instructions because there are not 
  //**            direct equivalent inside the new graphics (24 may 2006) 
  // xset('viewport', %XSHIFT, %YSHIFT) ; //** xset("viewport",x,y): Set the position of the panner. 
  
  scs_m.props.title(1) = 'Scilab Graphics of '+scs_m.props.title(1)
  
  drawobjs(scs_m, gh_curwin)
  //***********************************************************

  //** if pixmap then xset('wshow'),end
endfunction
