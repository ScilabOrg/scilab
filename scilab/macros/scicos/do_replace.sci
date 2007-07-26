function [scs_m,needcompile] = do_replace(scs_m, needcompile, Clipboard, Select)
// Copyright INRIA
// get replacement block
  
//** 02 May 2007 : change to new graphics
//** 24/07/07: Al@n's patch for rotation of blocks
  
  
  xc = %pt(1); yc = %pt(2);
  win = %win
  
  gh_curwin = scf(curwin) //** set the the current window and recover the handle
  
  //** SOURCE OBJECT Preparation 
  o = Clipboard ; //** "Clipboard arrives with the correct graphic object 
  //** if the object came from an existing diagram, I need to clear
  //** some graphics proprieties 
  graphics = o.graphics    //** change some graphics proprieties 
  graphics.pin   = 0*graphics.pin
  graphics.pout  = 0*graphics.pout
  graphics.pein  = 0*graphics.pein
  graphics.peout = 0*graphics.peout
  o.graphics = graphics

  //** DESTINATION OBJECT Preparation 
  k_n = Select(1,1)     ; //** the "Select" obj is the target 
  o_n = scs_m.objs(k_n) ;
    
  o.graphics.flip = o_n.graphics.flip // set same flip position
  o.graphics.theta = o_n.graphics.theta // set same theta value
  [ip,op,cip,cop] = (o.graphics.pin, o.graphics.pout, o.graphics.pein, o.graphics.peout)
  [in,out,clkin,clkout] = (o.model.in,o.model.out,o.model.evtin,o.model.evtout)
  
  nin  = size(in,'*')
  nout = size(out,'*')
  nclkin  = size(clkin,'*')
  nclkout = size(clkout,'*')
  
  [ip_n,op_n,cip_n,cop_n]=(o_n.graphics.pin,o_n.graphics.pout,..
			   o_n.graphics.pein,o_n.graphics.peout)
  [in_n,out_n,clkin_n,clkout_n]=(o_n.model.in,o_n.model.out,..
				 o_n.model.evtin,o_n.model.evtout)
  nin_n  = size(in_n,'*')
  nout_n = size(out_n,'*')
  nclkin_n  = size(clkin_n,'*')
  nclkout_n = size(clkout_n,'*')
  
  
  [ox,oy]     = getorigin(o)
  [ox_n,oy_n] = getorigin(o_n)
  
  
  [xx,yy,t] = getinputs(o);xx=xx-ox;yy=yy-oy
  [x_n,y_n,t_n] = getinputs(o_n); x_n = x_n-ox_n; y_n = y_n-oy_n
  
  // Check inputs
  wasconnected=%f
  if ip_n<>[] then if find(ip_n>0)<>[] then wasconnected=%t,end,end
  if wasconnected then
    if nin<>nin_n then
      message(['It is not yet possible to replace a block with'
	       'connected inputs with an other block with '
	       'different number of inputs'])
      ok=%f
      return
    elseif norm(xx(1:nin)-x_n(1:nin),1)>0.1|norm(yy(1:nin)-y_n(1:nin),1)>0.1 then
      message(['It is not yet possible to replace a block with'
	       'connected inputs with an other block with inputs not '
	       'relatively located in the same position'])
      ok=%f
      return
    else
      ip=ip_n
    end
  end
  
  // Check clock inputs
  wasconnected=%f
  if cip_n<>[] then if find(cip_n>0)<>[] then wasconnected=%t, end, end,
  if wasconnected then
    if nclkin<>nclkin_n then
      message(['It is not yet possible to replace a block with'
	       'connected clock inputs with an other block with '
	       'different number of clock inputs'])
      ok=%f
      return
    elseif norm(xx(t==-1)-x_n(t_n==-1),1)>0.1|norm(yy(t==-1)-y_n(t_n==-1),1)>0.1 then
      message(['It is not yet possible to replace a block with'
	       'connected clock inputs with an other block with clock inputs '
	       'not relatively located in the same position'])
      ok=%f
      return
    else
      cip=cip_n
    end
  end
  
  
  [xx,yy,t]=getoutputs(o);xx=xx-ox;yy=yy-oy
  [x_n,y_n,t_n]=getoutputs(o_n);x_n=x_n-ox_n;y_n=y_n-oy_n
  
  // Check outputs
  wasconnected=%f
  if op_n<>[] then if find(op_n>0)<>[] then wasconnected=%t, end, end,
  if wasconnected then
    if nout<>nout_n then
      message(['It is not yet possible to replace a block with'
	       'connected outputs with an other block with '
	       'different number of outputs'])
      ok=%f
      return
    elseif norm(xx(1:nout)-x_n(1:nout),1)>0.1|norm(yy(1:nout)-y_n(1:nout),1)>0.1 then
      message(['It is not yet possible to replace a block with'
	       'connected outputs with an other block with outputs not '
	       'relatively located in the same position'])
      ok=%f
      return
    else
      op=op_n
    end
  end
  
  // Check clock outputs
  wasconnected=%f
  if cop_n<>[] then if find(cop_n>0)<>[] then wasconnected=%t, end, end,
  if wasconnected then
    if nclkout<>nclkout_n then
      message(['It is not yet possible to replace a block with'
	       'connected clock outputs with an other block with '
	       'different number of clock outputs'])
      ok=%f
      return
    elseif norm(xx(t==-1)-x_n(t_n==-1),1)>0.1|norm(yy(t==-1)-y_n(t_n==-1),1)>0.1 then
      message(['It is not yet possible to replace a block with'
	       'connected clock outputs with an other block with clock outputs '
	       'not relatively located in the same position'])
      ok=%f
      return
    else
      cop=cop_n
    end
  end
  
  o.graphics.orig=[ox_n,oy_n]
  o.graphics.pin=ip
  o.graphics.pout=op
  o.graphics.pein=cip
  o.graphics.peout=cop
  
  //** OBSOLETE -> REMOVED 
  //** drawobj(o_n) // draw the new object 
 
  scs_m_save  = scs_m ; //** for "Undo" operation 
  scs_m.objs(k_n) = o ; //** update the object in diagram data structure  
 
  //** Graphics update    
  drawlater();
  //pause 
    drawobj(o); // draw the object: now the object is on the top of graphic handle list
    gh_obj = gh_curwin.children.children(1); //** get the handle of the graphics entity to modify
    gh_obj.children(1).mark_mode = "on"  ; //** update mark_mode property
    o_size = size(gh_curwin.children.children); //** dimension of graphics datastructure 
    gr_k_n = get_gri(k_n, o_size(1)) ; //** conversion from diagram to graphic
    swap_handles(gh_curwin.children.children(gr_k_n), gh_curwin.children.children(1));
    delete(gh_curwin.children.children(1)); //** delete the temp object 
  drawnow();
  show_pixmap();
  
  //** upper level variable update  
  nc_save     = needcompile //** need explanation 
  needcompile = 4
  [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save, nc_save, %t, %t)

endfunction
