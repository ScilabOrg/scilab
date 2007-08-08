function [%pt,scs_m] = do_tild(%pt,scs_m)
// Copyright INRIA
//** Alan 02/12/06 : use of objects swap in gh_curwin.children.children()
//**
//**  8 Aug 2007   : extended version  
//**
  win = %win ; //** acquire the window id
  
  gh_curwin = gh_current_window ; //** acquire the current window handler
  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object
  
  xc = %pt(1) ;
  yc = %pt(2) ;
  k = getblock(scs_m, [xc;yc]) ; //** try to recover the block 

  if k==[] then
    return ; //** if you click in the void : just return back
  end

  //** save the diagram for  
  scs_m_save = scs_m ; //** ... for undo ...
  
  
  //**--------- scs_m object manipulation -------------------
  
  path = list('objs',k)      ; //** acquire the index in the "global" diagram
  
  o = scs_m.objs(k) ;    //** scs_m
  
  o_n  = scs_m.objs(k) ; //** copy the old object in the new one 
  
  geom = o_n.graphics  ; //** isolate the geometric proprieties 
  
  geom.flip = ~geom.flip ; //** flip the object 
  
  o_n.graphics = geom; //** update the new object 
  
  scs_m = changeports(scs_m, path, o_n); 
  
 
  [scs_m_save, enable_undo, edited] = resume(scs_m_save, %t, %t)

endfunction
