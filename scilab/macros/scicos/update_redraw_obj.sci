function scs_m=update_redraw_obj(scs_m,path,o)
// Copyright INRIA

if length(path)==2 then
  
  if typeof(o)=='Link'|typeof(o)=='Text' then
    //** drawobj(scs_m(path)) ; //** XOR mode (old graphics) 
    scs_m(path)=o
    //** drawobj(scs_m(path)) ; //** new object 
  else
    scs_m = changeports(scs_m,path,o)
  end

else // change a block in a sub-level
  
  scs_m(path) = o ;

end

//**--- The quick and dirty solution of Simone -------------
  disablemenus() ; //** disable dynamical menu (just to be safe)
    gh_curwin = gh_current_window ; //** get the handle of the current graphics window
    clf( gh_curwin ) ; //** clear the current graphic window  
    %wdm = scs_m.props.wpar
    window_set_size() ; //** OK
    set_background()  ; //** OK
    
    drawlater() ;    
      drawobjs(scs_m)   ;  //** OK
    drawnow();show_pixmap() ;
  enablemenus() ;  //** re-enable dynamical menu 
  
endfunction
