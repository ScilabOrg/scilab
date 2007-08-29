function Details_()
//** INRIA
//** Comments by Simone Mannori

//** BEWARE 
//** Sporadic error report (right button activated ?)
//** 
//** !I recovered from the following error:                 !
//** !                                                      !
//** !TCL_EvalStr, invalid command name ".ss.t" at line 1   !
//** !                                                      !
//** !in Details_ action.                                   !

  Cmenu = [] ; 
  disablemenus()
    do_details(Select);
  enablemenus() ;
endfunction
