function hilite_obj(k,win)
// Copyright INRIA
// 
//** 21 Set 2006:
//** WARNING: I changed mechanism AND input values:
//**          from:
//**                hilite_obj(o,win)
//**          to
//**                hilite_obj(k,win)
//** 	             
//**          'o' was the object
//**          'k' IS the object INDEX inside 'scs_m' 

rhs = argn(2)
if rhs>1 then //** if the function is called with two arguments 
  
  //** check if "win" is an active window  
  if or(winsid()==win) then
    
    //** winback = xget('window')
    gh_winback = gcf(); //** save the handle of the sel. windows
    
    //**xset('window',win)
    gh_curwin = scf(win) ; 
  
  else
    return ; //** exit point: thw win specified is not in the SCICOS
  end        //** valid window

else //** use the default active window 
   
   gh_curwin = gcf() ; //** get the handle of the current figure
   win =  gh_curwin.figure_id ; //** get the 'win_id' of the current figure

end

//**-------------------------------------------------------------------

//** Too risky 
//** global Select; //** obtain local write access to the variable 

o_size = size ( gh_curwin.children.children ) ; 
gh_k = o_size(1) - k + 1 ; //** semi empirical equation :) 

drawlater();

  o = scs_m.objs(k); //** get the object 
  if typeof(o)=='Block' then
  

      //**------------------------------------------------
      //** Draw a 6 times thick rectangle around the designed object 
      //** graphics = o.graphics;
      //** [orig,sz] = (graphics.orig,graphics.sz)
      //** thick = xget('thickness') ;
      //** thick = xget('thickness')
      //** xset('thickness',6*thick);
      //** xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
      
      //**--- New Graphics --------------------------------
      gh_blk = gh_curwin.children.children(gh_k);
      gh_blk.children(1).visible = "on" ; //** active the selection markers
      Select = [Select ; k win ] ;  
      
    elseif typeof(o)=='Link' then
      //**--------------------------------------------------
      //** Increase 5 times the thickness of the link 
      //** o.thick(1) = 5*max(o.thick(1),1)
      //** drawobj(o)
      gh_blk = gh_curwin.children.children(gh_k);
      gh_blk.children(1).mark_mode = "on"  ; //** active the selection markers 
      Select = [Select ; k win ] ;  
      
  end

drawnow(); show_pixmap(); //** update the display   

//**-------------------------------------------------------------------

if rhs>1 then 
    //** xset('window',winback)
    scf(gh_winback); //** restore the active window 
end

endfunction
