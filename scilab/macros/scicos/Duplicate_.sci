function Duplicate_()
//** INRIA
//**
//** Comments by Simone Mannori 
//**

  Cmenu=[]; // Cmenu Quit can come out

  [scs_m,needcompile] = do_duplicate(%pt, scs_m, needcompile) ;
  
  %pt=[];

endfunction
