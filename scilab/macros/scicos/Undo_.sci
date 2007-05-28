function Undo_()
//** INRIA 
//**     
    Cmenu = [] ;
    %pt = [];
    
    if enable_undo then
       disablemenus()
         scs_m = scs_m_save ; //** recover the previous diagram
	 Select = [] ; //** unselect object  
         needcompile = nc_save ;
         %wdm = scs_m.props.wpar ;

         //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
         gh_curwin = gh_current_window ; //** get the handle of the current graphics window
	 scf(gh_curwin);
	 //** Get the current postion of the visible part of graphics in the panner. 
	 viewport  = xget("viewport"); //** [x,y] = xget("viewport")
         clf(gh_curwin) ;  //** clear the current graphic window  
         window_set_size(gh_curwin, viewport) ; //** OK
         set_background()  ; //** OK
         drawlater() ;
           //    disp("Start Timer ...."); timer(); 
           drawobjs(scs_m)   ;  //** OK
           //    disp ("End DrawObjs...."); disp(timer());  
         drawnow(); show_pixmap() ;
      
         enable_undo = %f ; //** "Undo" is a single level function 
      enablemenus()
    end
    
endfunction
