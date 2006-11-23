function gh_blk = drawobj(o, gh_window)
//
// Copyright INRIA
//!
//** Comments by Simone Mannori 
//
//** 22 May 2006 : this function IS substantially different from the standard
//**               SCICOS because came back with the graphic handler of 
//**               the compount that contains all the object(s) drawn. 
//**
//** 07 June 2006: adding of an empty box for the Select / HighLith operations 
//**
//** 22 Aug 2006: adding text support 
//**  
//** 21 Nov 2006: Save / Restore graphics attribute mechanism
//**
//** 23 Nov 2006: Faster Save/Restore, new "flat" object selection mechanism 
//**   
  rhs = argn(2) ; //** get the number of right side arguments  
  
  if rhs==1 then //** without arguments (default) assume ... 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = gh_current_window ; //** get the handle of the current graphics window     
     
  else //** the arguments is explicit 
     //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
     gh_curwin = gh_window ; //** get the handle of the current graphics window     

  end  
  
  gh_blk = [] ; //** create the empty object value 
  
  
  
  if typeof(o)=='Block' then //** Block draw 
    //** ---------------------- Block -----------------------------
  
    o_size = size ( gh_curwin.children.children ) ; //** initial size (number of graphic object
      
      
    //** Save graphics axes state 
      //** Save the state of the graphics window to avoid problems in case of not "well behaved"
      //** incorrect "gr_i" old graphics instructions 
      
      //** figure_background = gh_curwin.background  ;
      
      gh_axes = gh_curwin.children ;
      
      axes_font_style = gh_axes.font_style ;
      axes_font_size  = gh_axes.font_size  ;
      //** axes_font_color = gh_axes.font_color ; //** optional 
      
      //** axes_line_mode   = gh_axes.line_mode ; //** optional 
      axes_line_style  = gh_axes.line_style  ;
      axes_thickness   = gh_axes.thickness   ;
      
      //** axes_mark_mode       = gh_axes.mark_mode       ; //** optional 
      axes_mark_style      = gh_axes.mark_style      ;
      axes_mark_size       = gh_axes.mark_size       ;
      //** axes_mark_foreground = gh_axes.mark_foreground ; //** optional 
      //** axes_mark_background = gh_axes.mark_background ; //** optional 
      
      axes_foreground = gh_axes.foreground ;
      axes_background = gh_axes.background ; //** if not used cause some problem
 
   //**... end of figure and axes saving 
 
      //** Block drawing works throught call (execstr) the block function
      //** ... see "standard_draw" function  
      //** WARINING: this indirect "gr_i" execution can ruin the axis graphics proprieties because a not
      //**           formaly correct use of OLD graphics global primitives with "xset(..,..)"     
      ierr = execstr(o.gui+'(''plot'',o);','errcatch')
    
      [orig, sz, orient] = (o.graphics.orig, o.graphics.sz, o.graphics.flip) ;

   //** Restore graphics axes state 
      //** Restore the state of the graphics window 
      
      gh_axes.font_style = axes_font_style ;
      gh_axes.font_size  = axes_font_size  ;
      //** gh_axes.font_color = axes_font_color ; //** optional 
      
      //** gh_axes.line_mode   = axes_line_mode   ; //** optional 
      gh_axes.line_style  = axes_line_style  ;
      gh_axes.thickness   = axes_thickness   ;
      
      //** gh_axes.mark_mode       = axes_mark_mode       ; //** optional 
      gh_axes.mark_style      = axes_mark_style      ;
      gh_axes.mark_size       = axes_mark_size       ;
      //** gh_axes.mark_foreground = axes_mark_foreground ; //** optional 
      //** gh_axes.mark_background = axes_mark_background ; //** optional 
      
      gh_axes.foreground = axes_foreground ;
      gh_axes.background = axes_background ;
                  
      //** gh_curwin.background = figure_background ;

   //**... end of figure and axes state restoring  
        
      //** Add the 'select' box and put "mark_mode" off, ready for 'Select' operation
      sel_x = orig(1) ; sel_y = orig(2)+sz(2) ;
      sel_w = sz(1)   ; sel_h = sz(2)   ;
       xrect(sel_x, sel_y, sel_w, sel_h);
       gh_e = gce()              ; //** get the "select box" handle
       gh_e.mark_background = -1 ; //** 
       gh_e.mark_style = 11      ;
       gh_e.mark_mode = "off"    ; //** used 
       gh_e.line_mode = "off"    ;
       // gh_e.visible = "off"      ; //** put invisible 
 
    p_size = size ( gh_curwin.children.children ) ; //** size after the draw 
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1) ;
    gh_blk = glue( gh_curwin.children.children(d_size:-1:1) ) ; 

    if ierr<>0 then 
      message(['Block '+o.gui+ ' not defined'; 'You must leave scicos and define it now.']) ;
      gh_blk = [];
    end
    
  //** ---------- Link -------------------------------    
  elseif typeof(o)=='Link' then //** Link draw 
  
    o_size = size ( gh_curwin.children.children ) ; //** initial size
      
        xpoly(o.xx, o.yy,'lines')  ; //** draw the polyline "Link" 
        gh_e = gce()               ;
        gh_e.thickness = maxi( o.thick(1) , 1) * maxi(o.thick(2), 1) ; //** thickness
        gh_e.foreground = o.ct(1)  ; //** link color
	gh_e.mark_style = 11       ;  
        gh_e.mark_mode = "off"     ; //** put "mark_mode" off, ready for 'Select' operation
	
    p_size = size ( gh_curwin.children.children ) ; //** size after the draw 
    //** aggregate the graphics entities
    d_size = p_size(1) - o_size(1) ;
    gh_blk = glue( gh_curwin.children.children(d_size:-1:1) ) ; 
     
  //** ---------- Deleted ----- CAUTION: also "Deleted" object MUST be draw ! ----    
  elseif typeof(o)=='Deleted' then //** Dummy "deleted" draw     
    
    //** draw a dummy object 
    
    xpoly ([0;1],[0;1]) ; //** just a dummy object 
    gh_blk = glue( gh_curwin.children.children(1) ) ; //** create the Compound macro object 
    
    //** gh_blk.visible = "off"  ; //** set to invisible :)
    set (gh_blk,"visible","off"); //** set to invisible -> faster version 
    
     
  //** ---------- Text -------------------------------  
  elseif typeof(o)=='Text' then //** Text draw 
    
    //**  ------ Put the new graphics here -----------
    //**
    o_size = size ( gh_curwin.children.children ) ; //** initial size
       execstr(o.gui+'(''plot'',o)') ;
    p_size = size ( gh_curwin.children.children ) ; //** size after the draw 
    //** aggregate the graphics entities
    d_size =  p_size(1) - o_size(1) ;
    gh_blk = glue( gh_curwin.children.children(d_size:-1:1) ) ; 
  end //** of the main if  
  
endfunction
