function Zoomin_()
//** INRIA
//** 12 Feb. 2006:  new graphic (Simone), grid insertion (Alan)
//**                geometrical correction (Ramine)

    Cmenu = [];
    xinfo('Zoom in');

    gh_window = gh_current_window; 

    zoomfactor = 1.2
    %zoom = %zoom * zoomfactor;

    drawlater();
    disablemenus();
    //** Get the current postion of the visible part of graphics in the panner.
    viewport = xget("viewport"); //** [x,y] = xget("viewport")

    //** geometrical correction: zoom in the center
    viewport = viewport * zoomfactor - 0.5 * gh_window.figure_size*(1-zoomfactor)  ;
    window_set_size(gh_window, viewport);
    if exists('%scicos_with_grid') then
      drawgrid(); //** draw the new grid and put in the bottom of stack
      swap_handles(gh_window.children.children($), gh_window.children.children(1));
      delete(gh_window.children.children(1)); //** delete the old grid
    end
    ChangeTextFont(gh_window)
    drawnow();
    show_pixmap();

    enablemenus();

    xinfo(' ');
endfunction
