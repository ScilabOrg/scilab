function restore(gh_curwin)
      //gh_curwin = scf(curwin);
      gh_current_window = gh_curwin ; //** ... non ne sono proprio sicuro ....
                                      //** ... pero' sembra che funzioni .....
      clf(gh_curwin) ;
      gh_curwin.pixmap = "on"

      if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
        scs_m.props.options('3D')(1)=%f //disable 3D block shape
      end

      set_background(gh_curwin)   ;
      pwindow_set_size(gh_curwin) ;
      window_set_size(gh_curwin)  ;


      menu_stuff() ;

      if ~super_block then
	           delmenu(curwin,'stop')
	           addmenu(curwin,'stop',list(1,'haltscicos'))
	           unsetmenu(curwin,'stop')
      else
	unsetmenu(curwin,'Simulate')
      end
endfunction
