function standard_draw (o,frame,draw_ports,up)
//
// Copyright INRIA
//
//** Comments and mods by Simone Mannori 
//** 19 Jan 2006
//** This function is used to draw the object from the most of (but NOT all)
//** interface functions 
  
  xf = 60 ; yf = 40 ;
  
  [lhs,rhs] = argn(0)

  if rhs==1 then
      frame=%t
  end

  if rhs<3 then
      draw_ports = standard_draw_ports     //** left right ports
                                           //** the function 'standard_draw_ports' it's copied in 'draw_ports'
  elseif rhs==4 then                       //** otherwise 
      draw_ports = standard_draw_ports_up  //** up / down ports 
  end                                      //** the function 'standard_draw_ports_up' it's copied in 'draw_ports'

  nin  = size(o.model.in,1);
  nout = size(o.model.out,1);
  clkin = size(o.model.evtin,1);
  clkout= size(o.model.evtout,1);
  [orig,sz,orient] = (o.graphics.orig,o.graphics.sz,o.graphics.flip)

//** Default values -------------------
  thick = 1              ; //** patch Simone 
  pat = default_color(0) ; //**   
  e = 4 ;
  With3D = options('3D')(1)
//** ----------------------------------

//** local haldler(read/write) = semiglobalhandler(read/only)  
  gh_curwin = gh_current_window     ; //** get the handle of the current graphics window
  background = gh_curwin.background ; 
 
  
  gr_i = o.graphics.gr_i

  if type(gr_i) == 15 then 
     [gr_i,coli] = gr_i(1:2) ;
  else
      coli = [] ;
  end

  // draw box
  if frame then 

    if With3D then  //** this is the code relative to the block's "window dressing" 
      //**---------- 3D Mode ON -----------------------------------------------------------------------
      #Color3D = options('3D')(2)      
      // xpoly([orig(1)+e;orig(1)+sz(1);orig(1)+sz(1)],[orig(2)+sz(2);orig(2)+sz(2);orig(2)+e],'lines')
      xrect( orig(1)+e, orig(2)+sz(2), sz(1)-e, sz(2)-e) ;
      gh_e = gce(); //** new graphics :)
      gh_e.thickness = 0 ;  
      gh_e.foreground = #Color3D ;
      
      if coli<>[] then 
          gh_e.fill_mode = "on"  ;
          gh_e.background = coli ;
      end
      
      eps = 0.3 ;
      
      xx = [ orig(1)   , orig(1)
	     orig(1)   , orig(1)+sz(1)-e
	     orig(1)+e , orig(1)+sz(1)
	     orig(1)+e , orig(1)+e] ;
      
      yy = [orig(2)         , orig(2)
	    orig(2)+sz(2)-e , orig(2)
	    orig(2)+sz(2)   , orig(2)+e
	    orig(2)+e       , orig(2)+e];     
      
      xfpolys(xx,yy,-[1,1]*#Color3D); //** fill a set of polygons
      gh_e = gce()              ;              
      gh_c = gh_e.children(1:2) ;
      gh_c.foreground = default_color(0) ;
      gh_c.thickness = 2 ;
      
    else //** not in 3D mode 
      //**----------3D Mode OFF -------------------------------------------------------------------------
      e = 0   
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
      gh_e = gce();
      gh_e.thickness = 2 ;
      
      if coli<>[] then 
          gh_e.fill_mode = "on"  ;
          gh_e.background = coli ;
      end
      
      
    end //** of 3D mode ON/OFF  
      
  end //** of the frame mode 

  draw_ports(o) ; //** 'standard_draw_ports' or 'standard_draw_ports_up'

//** --------------------------------------------------------------------------------------------------------------

//** -------------------------------- Identification --------------------------------------------------------------

 //------------------------
  ident = o.graphics.id
  
  //** fnt = xget('font') ; //** store the current/default font type/size in order to restore later  
  gh_winpal = gca(); //** get the Axes proprieties 
 
  default_font_style = gh_winpal.font_style ;
  default_font_size  = gh_winpal.font_size  ;
  default_font_color = gh_winpal.font_color ;
  
  // draw Identification
  if ident <> []& ident <> ''  then
      //** xset('font', options.ID(1)(1), options.ID(1)(2)) ;//** old graphics 
      gh_winpal.font_style = options.ID(1)(1) ; 
      gh_winpal.font_size  = options.ID(1)(2) ;
      //** font color not yet used 
      rectangle = xstringl(orig(1), orig(2), ident) ;
      w = max(rectangle(3), sz(1)) ;
      h = rectangle(4) * 1.3 ;
      xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;
      //** xset('font', fnt(1), fnt(2)) ; //** restore font attribute
      gh_winpal.font_style = default_font_style ;
      gh_winpal.font_size  = default_font_size  ;
      gh_winpal.font_color = default_font_color ; 
  end

  deff('c=scs_color(c)',' ') ; //** on line function definition 
  
  flag = 'foreground'
    
  gr_i = [gr_i ] ; //** simple but not simpler :) 
        
  model = o.model
  
  if With3D&frame then
    orig = orig+e
    sz = sz-e
  end

//** Phisical draw AND check of the graphics object   
  if execstr(gr_i,'errcatch')<>0 then 
      message(['Error in Icon defintion'; 'See error message in scilab window'])
  end
    
  //** restore the default font after an eventual gr_i modification 
  //** xset('font',fnt(1),fnt(2))
   gh_winpal.font_style = default_font_style ;
   gh_winpal.font_size  = default_font_size  ;
   gh_winpal.font_color = default_font_color ;

   gh_curwin.background = background; 
   
   //** I use - temporally - an old graphics instruction to fix a bug 
   xset('thickness',1)
   
  //** Obsolete (old Graphics)
  // xset('thickness',1)
  // xset('pattern',pat)

endfunction

