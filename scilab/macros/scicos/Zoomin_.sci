function Zoomin_()
    Cmenu = [];
    xinfo('Zoom in');
    %zoom=%zoom*1.2;
    gh_window = gcf();
    drawlater();
    disablemenus();
    window_set_size(gh_window);
    drawnow();
    show_pixmap();
    enablemenus();
    xinfo(' ');
endfunction
