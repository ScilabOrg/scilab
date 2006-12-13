function [%pt,scs_m]=do_tild(%pt,scs_m)
// Copyright INRIA
//** 29 June 2006 : waiting for the random access to the graphics datastructure
//**                I force a Replot after the 
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()

  win = %win;

  gh_curwin = gh_current_window ; //** acquire the current window handler 
  gh_curwin.pixmap ='on';         //** force pixmap mode

  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object

  xc = %pt(1) ; yc = %pt(2)   ;

  k = getblock(scs_m,[xc;yc]) ;

  if k==[] then return , end ; //** if you click in the void ... return back 

  if get_connected(scs_m,k)<>[] then //** see message
     hilite_obj(k); //** new
     message('Connected block can''t be tilded')
     unhilite_obj(k); //** new
     return
  end

  //**--------- scs_m object manipulation -------------------
  o = scs_m.objs(k) ; //** scs_m
  geom = o.graphics ; geom.flip = ~geom.flip ; o.graphics = geom;

  //**---------------------------------------------------------
  gr_k = o_size(1) - k + 1 ; //** semi empirical equation :)
  drawlater();
  update_gr(gr_k,o)
  drawnow(); show_pixmap() ;
  //**------------------------------------------------------------

  scs_m_save = scs_m ; //** ... for undo ...

  scs_m.objs(k)=o

  [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)

  //** ... force a Replot() in the calling function 

endfunction
