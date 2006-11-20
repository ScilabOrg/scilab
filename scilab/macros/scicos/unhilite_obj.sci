function unhilite_obj(o,win)
//
// Copyright INRIA
rhs = argn(2)
if rhs>1 then
  
  if or(winsid()==win) then
    
    //** winback=xget('window')
    gh_winback = gcf(); //** save the handle of the sel. windows
    
    //** xset('window',win) 
    gh_win = scf(win) ; 
    
    //** alu = xget('alufunction') //** save the mode
    //** xset('alufunction',6)     //** pass to the xor mode 
                                   //** ---> XOR MODE FROM HERE 
  else
    return; //** exit point: the 'win' specified is not in the SCICOS
  end       //** valid window
end

//** dr = driver()
//** if dr=='Rec' then driver('X11'),end

if typeof(o)=='Block' then
  
  graphics = o.graphics ;
  [orig, sz] = (graphics.orig, graphics.sz) ;
  
  //**------------------------------------------------
  //** Draw a 6 times thick rectangle around the designed object 
  //** thick = xget('thickness') ;
  //** xset('thickness',6*thick);
  //** xrect(orig(1), orig(2)+sz(2), sz(1), sz(2));
  //** if pixmap then xset('wshow'),end  
  //** xset('thickness',thick);

elseif typeof(o)=='Link' then
  //**--------------------------------------------------
  //** Intrease 5 times the thickness of the link 
  //** o.thick(1)=5*max(o.thick(1),1)
  //** drawobj(o)
  //** if pixmap then xset('wshow'),end

end

//** driver(dr)  

if rhs>1 then 
  //** xset('alufunction',alu)
  //** xset('window',winback)
  scf(gh_winback); //** restore the active window 
end

endfunction
