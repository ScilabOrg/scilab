function Paste_()
//** INRIA 
//** 02 May 2007: Update the paste operation with "Replace" (paste over) operation 


//** Verify window and last mouse "click" position 
  if %win<>curwin  then
     disp("Paste operation is not possible in this window ")
     return
  end        
    
//** if I am in the current window and the mouse position are OK     
    
  gh_curwin = scf(curwin); //** put the focus in the current window and recover the handle
  drawlater(); //** put in "drawlater" mode 
  
//** Select : matrix of selected object
//** Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
//**                                 and "win_id"    is the Scilab window id.
//** Multiple selection is permitted: each object is a line of the matrix.  

//** Check for "Replace" or "Paste in the void" datastrucure 
   
if and(size(Select)==[1,2]) then //** only one object selected 
  
  Sel_obj = scs_m.objs(Select(1,1)) ; 
  
  if (typeof(Clipboard)=='Block' & typeof(Sel_obj)=='Block')
   //** ready for "Replace" operation 
   
   // This case is used when trying to replace a block by itself
   // It does a duplicate to be Simulink/Dymola compatible
   if and(Clipboard.graphics.sz==Sel_obj.graphics.sz)& ...
	 (Clipboard.graphics.orig==Sel_obj.graphics.orig) then
     scs_m_save = scs_m; nc_save = needcompile ;
     
      blk = Clipboard
      blk.graphics.orig = Clipboard.graphics.orig+Clipboard.graphics.sz/2;
      scs_m.objs($+1) = blk //** add the object at the top 
      drawobj(blk); //** draw the single object 
      edited = %t
      enable_undo = %t
      Select = [size(scs_m.objs),%win]; //** it's a really dirty trick ;)
                                        //** because the pasted object is the last ;)
     else
       [scs_m, needcompile] = do_replace(scs_m, needcompile, Clipboard,Select)
   end
   
  else
   disp("Paste -> Source / Destination incompatible"); 
   Cmenu=[]; %pt = []; %ppt = [] ; return 
  end
  
else   
    
    if typeof(Clipboard)=='Block'| typeof(Clipboard)=='Text' then
      
      scs_m_save = scs_m; nc_save = needcompile ;
      if %ppt==[] then
	%ppt=Clipboard.graphics.orig+Clipboard.graphics.sz/2;      
      end  
      
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
      if %ppt==[] then
	for i=1:size(Clipboard.objs)
	  if typeof(Clipboard.objs(i))=="Block" then 
	    if %ppt==[] then
	      %ppt(1)=Clipboard.objs(i).graphics.orig(1);%ppt(2)=Clipboard.objs(i).graphics.orig(2);
	    else
	      %ppt(1)=min(%ppt(1),Clipboard.objs(i).graphics.orig(1));
	      %ppt(2)=min(%ppt(2),Clipboard.objs(i).graphics.orig(2));
	    end,
	  end
	end
	if %ppt==[] then
	  disp('No block is selected.');
	  Cmenu=[]; %pt = []; %ppt = [] ; return 
	else
	  %ppt=%ppt+10   // decalage, a modifier
	end
      end
      
      
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
	  
	  drawobj(o);   //** draw the object 
	
	end
	//**------------------------------------------------------
	needcompile = 4 ;
	enable_undo = %t;
	edited = %t     ;
      end //** more than one object is pasted 
    
    end //** object type 
  
  show_pixmap(); //** update the screen 
  
  
 end //** valid Paste as "replace" or "in the void"
  
  Cmenu=[]; %pt = []; %ppt = [] ;
  
endfunction
