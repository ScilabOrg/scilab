function Export_()
//** INRIA     
//**    
  disablemenus()
  do_export(scs_m) ;
  scf(gh_current_window); 
  drawnow() ; show_pixmap() ;
  enablemenus()
  Cmenu=[]   ;
endfunction
