function IconEditor_()
//** INRIA
  scs_m_save = scs_m    ;
  nc_save = needcompile ;
  enable_undo = %t      ;

  [scs_m] = do_icon_edit(%pt,scs_m)
  Cmenu = [] ;
  %pt   = [] ;

  edited = %t ;


  drawnow(); show_pixmap() ;      //** re-draw the graphic object and show on screen

endfunction
