function Zoomout_()
//** INRIA
//** 24 Aug 2006: Simone Mannori
    Cmenu = [];
    xinfo('Zoom out');
    %zoom = %zoom / 1.2;
    gh_window = gcf();
    drawlater();
    disablemenus();
    window_set_size(gh_window);
    drawgrid();
    swap_handles(gh_window.children.children($),...
                 gh_window.children.children(1));
    delete(gh_window.children.children(1));
    drawnow();
    show_pixmap();
    enablemenus();
    xinfo(' ');
endfunction
