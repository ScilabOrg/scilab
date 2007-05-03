function do_help(%pt)
// Copyright INRIA
//**
//** 03 May 2007: BEWARE : this code handle the "help" only for Scicos block (no link, no menu)  
//** 
//** nm = 0 ; -> the help is relative to a block 
//**
//** nm = 1 ; -> the help is realative to a menu: this operating mode is not yet implemented 
//**

  cwin = %win ; //** acquire the current window  
  
  //** if the block is in the current window 
  if cwin==curwin then
    xc = %pt(1); yc = %pt(2) ;
    k = getobj(scs_m,[xc;yc]);
    if k<>[] then 
	o = scs_m.objs(k); //** recover the Scicos object 
	if typeof(o)=='Block' then 
	  name = o.gui ; //** recover the block's name 
	  nm = 0       ; //** help on block 
	else
	  return; //** EXIT: it is not a block
	end    
    else
      return; //** EXIT: no valid object is found 
    end
  
  //** if the object is in a palette 
  elseif or(windows(find(windows(:,1)<0),2)==cwin) then
    kwin = find(windows(:,2)==cwin)
    pal  = palettes(-windows(kwin,1))
    xc = %pt(1); yc = %pt(2);
    k  = getobj(pal,[xc;yc])
    if k<>[] then
        o = pal.objs(k)
        if typeof(o)=='Block' then
          name = o.gui
          nm = 0;
	else
	  return; //** EXIT: it is not a block
	end 
    else
      return; //** EXIT
    end
  
  //** it is not a Scicos Window 
  else
      return; //** EXIT
  end

  //** 
  if nm==0 then //** only for block in current window or palette 
    help(name) ; //** this is a Scilab primitive that show the help page relative to "name" item 
    return; //** EXIT
  end

//**---------------------------------------------------------------------------------------------  
//** This old:obsolete code is not yet usable. It is here for historical reason
//** We hope to modify the functionality in ordert to recover the informations stored inside 
//** "%scicos_help(name)" string array
//**
//**  if execstr('mess=%scicos_help(name)','errcatch') <>0 then
//**    mess = "No help available on this topic. Sorry.";
//**  end
//**  message(mess); 
    
endfunction
