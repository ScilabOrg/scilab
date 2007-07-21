function [scs_m]=do_turn(%pt,scs_m,theta)
// Copyright INRIA
//** x,07/07 Alan : do_turn update theta value of blocks
//**                and redraw it

  win = %win;

  gh_curwin = gh_current_window ; //** acquire the current window handler
  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object
  xc = %pt(1) ; yc = %pt(2)   ;
  k = getobj(scs_m,[xc;yc]) ;

  //**--------- check k and scs_m.objs(k) ------------------
  if k==[] then
    return
  end //** if you click in the void ... return back

  o = scs_m.objs(k)
  if typeof(o)=='Link' then
    return
  end //**disable rotation for link

  if get_connected(scs_m,k)<>[] then //** see message
     hilite_obj(k); //** new
     message('Connected block can''t be turned')
     unhilite_obj(k); //** new
     return
  end

  //**--------- scs_m theta update -------------------------
  geom = o.graphics ;
  geom.theta = geom.theta + theta
  while geom.theta>360 then
    geom.theta=geom.theta-360;
  end
  while theta<-360 then
    geom.theta=geom.theta+360;
  end
  o.graphics = geom;

  //**------ draw object in new graphics structure ---------
  gr_k=get_gri(k,o_size(1))
  drawlater();
  update_gr(gr_k,o)
  draw(gh_curwin.children);
  show_pixmap() ;

  scs_m_save = scs_m ; //** ... for undo ...
  scs_m.objs(k)=o

  [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)

endfunction
