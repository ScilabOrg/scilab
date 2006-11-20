function Zoomin_()
    
    Cmenu = [] ;
    
    xinfo('Zoom in')  ;
    
    %zoom=%zoom*1.2   ;
    
    gh_window = gcf() ;
    
    clf(gh_window); //** clear current graphic window and delete all his children
    
    set_background(gh_window) ; //** --> 
    
    window_set_size(gh_window) ;
    
    drawobjs(scs_m)   ;
    
    xinfo(' ')        ;

endfunction
