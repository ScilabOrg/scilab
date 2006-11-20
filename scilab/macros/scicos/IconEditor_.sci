function IconEditor_()
//** INRIA     
    scs_m_save = scs_m    ;
    nc_save = needcompile ;
    enable_undo = %t      ; 
    
    disablemenus()
     [scs_m] = do_icon_edit(%pt,scs_m)
     Cmenu = [] ;
     %pt   = [] ;
    enablemenus()
    
    edited = %t ; 
    
  // Acquire the current clicked window and put to "on" the pixmap mode
  gh_curwin = scf(%win)    ;
  gh_curwin.pixmap = "on"  ; //** just for safety

  //** Clear the graphic window WITHOUT changing his pamaters ! :)
  drawlater() ;
     delete(gh_curwin.children.children) ; //** wipe out all the temp graphics object
     drawobjs(scs_m, gh_curwin) ;   //** re-create all the graphics object     
  drawnow(); show_pixmap() ;      //** re-draw the graphic object and show on screen   
  
  Cmenu = [] ; %pt = [];
       
endfunction
