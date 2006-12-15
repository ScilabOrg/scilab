function AboutSCICOS_()
//   INRIA 
//** 21 Nov 2006 
//** Comments by Simone Mannori 

    Cmenu=[] ; %pt=[];
    
    disablemenus() ; //** disable dynamical menu (just to be safe)
    
     x_message([strsubst(scicos_ver,"scicos","SCICOS -")+"-"]);
    
    enablemenus() ;  //** re-enable dynamical menu 

endfunction
