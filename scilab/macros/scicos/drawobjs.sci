function drawobjs(scs_m, gh_window)
// Copyright INRIA
//**
 
  drawlater() ;

    rhs = argn(2) ; //** get the number of right side arguments  
  
    if rhs==1 then //** without arguments (default) assume ... 
       //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
       // gh_curwin = gh_current_window ; //** get the handle of the current graphics window     
       for i=1 : lstsize(scs_m.objs) //** draw object by object 
            gh_blk = drawobj(scs_m.objs(i));
       end
    else //** the argument is explicit 
       //** It is NOT possible to modify [gh_current_window] directly outside [scicos_new]
       gh_curwin = gh_window ; //** get the handle of the current graphics window     
       for i=1 : lstsize(scs_m.objs) //** draw object by object 
          gh_blk = drawobj(scs_m.objs(i), gh_curwin);
       end
    end

  drawnow(); show_pixmap() ;
  
  drawtitle(scs_m.props)
  
  show_info(scs_m.props.doc)

endfunction
