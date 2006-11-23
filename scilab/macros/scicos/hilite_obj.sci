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
//**
//** 23 Nov 2006: New mechanism that use ONLY the "mark_mode" ["on","off"]
//**              graphics attribute for BOTH "Block" and "Link"
//**

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

o_size = size ( gh_curwin.children.children ) ; 
gh_k = o_size(1) - k + 1 ; //** semi empirical equation :) 

drawlater();

      gh_blk = gh_curwin.children.children(gh_k);
      gh_blk.children(1).mark_mode = "on"  ; //** active the selection markers 
      Select = [Select ; k win ] ;  

drawnow(); show_pixmap(); //** update the display   

//**-------------------------------------------------------------------

if rhs>1 then 
    //** xset('window',winback)
    scf(gh_winback); //** restore the active window 
end

endfunction
