function Paste_()
//** INRIA 
//** 
  //** if I am in the current window and the mouse position are OK 
  if (%win==curwin) & (%ppt<>[]) then
    scf(curwin); gh_curwin = gcf(curwin);
    drawlater(); 
    
    //** if it is a "Block" or a "Text"
    if typeof(Clipboard)=='Block'| typeof(Clipboard)=='Text' then
      
      scs_m_save = scs_m,nc_save = needcompile ;
      
      blk = Clipboard
      blk.graphics.orig = %ppt
      scs_m.objs($+1) = blk //** add the object at the top 
      drawobj(blk); //** draw the single object 
      edited = %t
      enable_undo = %t
      Select = [size(scs_m.objs),%win]; //** it's a really dirty trick ;)
                                        //** because the pasted object is the last ;)
    
    //** if it is a Diagram (Block, Text and Link)
    elseif  typeof(Clipboard)=='diagram' then
      reg = Clipboard;
      Clipboard = list()  // region is not persistent
      
   
      if size(reg.objs)>=1 then
	Select = []; //** clear the data structure
	scs_m_save = scs_m,nc_save = needcompile
	n = lstsize(scs_m.objs)
	xc = %ppt(1),yc=%ppt(2)
	rect = dig_bound(reg)
	
	//**----------------------------------------------------
	//** scan all the obj of the diagram to paste 
	for k=1:size(reg.objs)
	  o = reg.objs(k)
	  
	  // translate blocks and update connection index 
	  if typeof(o)=='Link' then
	    o.xx = o.xx-rect(1)+xc
	    o.yy = o.yy-rect(2)+yc
	    [from,to] = (o.from,o.to)
	    o.from(1) =  o.from(1)+n;
	    o.to(1) = o.to(1)+n;
	  elseif typeof(o)=='Block' then
	    o.graphics.orig(1)=o.graphics.orig(1)-rect(1)+xc
	    o.graphics.orig(2)=o.graphics.orig(2)-rect(2)+yc
	    k_conn=find(o.graphics.pin>0)
	    o.graphics.pin(k_conn)=o.graphics.pin(k_conn)+n
	    k_conn=find(o.graphics.pout>0)
	    o.graphics.pout(k_conn)=o.graphics.pout(k_conn)+n
	    k_conn=find(o.graphics.pein>0)
	    o.graphics.pein(k_conn)=o.graphics.pein(k_conn)+n
	    k_conn=find(o.graphics.peout>0)
	    o.graphics.peout(k_conn)=o.graphics.peout(k_conn)+n
	  elseif typeof(o)=='Text' then
	    o.graphics.orig(1)=o.graphics.orig(1)-rect(1)+xc
	    o.graphics.orig(2)=o.graphics.orig(2)-rect(2)+yc
	  end
	  
	  scs_m.objs($+1) = o ;
	  Select = [Select ; size(scs_m.objs) , %win]; //** it's a really dirty trick ;)
                        //** because the pasted object is the last ;)
	  
	  drawobj(o); //** draw the now object 
	
	end
	//**------------------------------------------------------
	needcompile = 4 ;
	enable_undo = %t;
	edited = %t     ;
      end //** more than one object is pasted 
    
    end //** object type 
  
  show_pixmap(); //** update the screen 
  end //** valid paste 
  
  Cmenu=[]; %pt = []; %ppt = [] ;
  
//  draw(gh_curwin.children); show_pixmap(); //** update the screen 

endfunction
