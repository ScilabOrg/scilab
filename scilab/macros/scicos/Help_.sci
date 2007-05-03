function Help_()
//** INRIA
//**
//** 03 May 2007: BEWARE : this code handle the "help" only for Scicos block (no link, no menu)  
//**  


  do_help(%pt) ; //** call the help function 
  
//** Clear state machine variables   
  Cmenu = [];
  %pt = [];
endfunction
