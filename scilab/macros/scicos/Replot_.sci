function Replot_()
//   INRIA
//** 24 may 2006, Comments by Simone Mannori
    Cmenu=[] ;
    %pt=[];
    disablemenus() ; //** disable dynamical menu (just to be safe)
    gh_curwin = gh_current_window ; //** get the handle of the current graphics window
    drawlater();
    clf(gh_curwin) ; //** clear the current graphic window
    %wdm = scs_m.props.wpar
    window_set_size(gh_curwin) ; //** OK
    set_background(gh_curwin)  ; //** OK
    drawobjs(scs_m)   ;  //** Redraw all
    drawnow(); show_pixmap();
    enablemenus() ;
endfunction
