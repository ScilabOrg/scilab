function Replot_()
//   INRIA 
//** 24 may 2006 
//** Comments by Simone Mannori 

    Cmenu=[] ; %pt=[];
    
    disablemenus() ; //** disable dynamical menu (just to be safe)
    
    //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
    //** "gh_current_window" is a "semiglobal variable"
    gh_curwin = gh_current_window ; //** get the handle of the current graphics window
    
    clf(gh_curwin) ; //** clear the current graphic window  
    
    %wdm = scs_m.props.wpar
   
    window_set_size(gh_curwin) ; //** OK
    
    set_background(gh_curwin)  ; //** OK
    
//    disp("Start Timer ...."); timer(); 
    
      drawobjs(scs_m)   ;  //** Redraw all 
    
//    disp ("End DrawObjs...."); disp(timer());  
    
//    disp("End Show... ");disp(timer());  
    
    enablemenus() ;  //** re-enable dynamical menu 

endfunction
