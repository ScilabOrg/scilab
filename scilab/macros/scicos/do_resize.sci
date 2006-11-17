function [%pt,scs_m]=do_resize(%pt,scs_m)
// Copyright INRIA
  win=%win;
  if Select==[] then
    xc=%pt(1);yc=%pt(2);;%pt=[]
    K=getblocklink(scs_m,[xc;yc])
  else
    K=Select(:,1)';%pt=[]
    if size(K,'*')>1|%win<>Select(1,2) then
      message("Only one block can be selected in current window for this operation.")
      Cmenu=[];%pt=[];return
    end
  end
 
    
    
    
  if K<>[] then 
    if typeof(scs_m.objs(K))=='Block' then
      
    else
      [pos,ct]=(scs_m.objs(K).thick,scs_m.objs(K).ct)
      Thick=pos(1)
      Type=pos(2)
      [ok,Thick,Type]=getvalue('Link parameters',['Thickness';'Type'],..
			       list('vec','1','vec',1),[string(Thick);string(Type)])
      if ok then
	drawobj(scs_m.objs(K))
	edited=or(scs_m.objs(K).thick<>[Thick,Type]);
	
	scs_m.objs(K).thick=[Thick,Type];
	drawobj(scs_m.objs(K))
      end
      return
    end
  else
    Cmenu=[];return
  end
  
  o=scs_m.objs(K)
  
  graphics=o.graphics
  sz=graphics.sz
  orig=graphics.orig
  [ok,w,h]=getvalue('Set Block sizes',['width';'height'],..
		    list('vec',1,'vec',1),string(sz(:)))
  if ok  then
    w=maxi(w,5)
    h=maxi(h,5)
    
    if w<>sz(1) then
      if [get_connected(scs_m,K,'out'),..
	  get_connected(scs_m,K,'clkin'),..
	  get_connected(scs_m,K,'clkout')]<>[] then 
	message(['Block with connected standard port outputs'
		 'or Event ports cannot be resized horizontally'])
	return
      end
    end
    if h<>sz(2) then
      if [get_connected(scs_m,K,'out'),..
	  get_connected(scs_m,K,'in'),..
	  get_connected(scs_m,K,'clkin')]<>[] then 
	message(['Block with connected standards ports'
		 'or Event input ports cannot be resized vertically'])
	return
      end
    end
    graphics.sz=[w;h]
    graphics.orig=orig
    
    drawblock(o)
    o.graphics=graphics
    scs_m.objs(K)=o
    drawblock(o)
  end
endfunction
