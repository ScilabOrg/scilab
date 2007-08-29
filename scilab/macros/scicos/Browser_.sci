function Browser_()
//** INRIA
//** Comments by Simone Mannori

Cmenu = [] ; 

if super_path==[] then
  disablemenus();
   do_browser(scs_m);
  enablemenus();  
else
  message("Browser can only be launched from main diagram.") ;
  //** I would change this message to:
  //**  "Browser can only be launched from main diagram."
  //**  "Please close all the super blocks and launch"
  //**  "The Browser from the main diagram"
end

endfunction
