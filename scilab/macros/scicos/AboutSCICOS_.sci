function AboutSCICOS_()
//   INRIA 
//** 21 Nov 2006 
//** Comments by Simone Mannori 

    Cmenu=[] ; %pt=[];
    
    disablemenus() ; //** disable dynamical menu (just to be safe)
    
     x_message(["Scicos is the best simulation programs that...";
                "    you can find for this money :) .";
                "             "+...
                    strsubst(scicos_ver,"scicos","SCICOS -")+"-"]);
    
    enablemenus() ;  //** re-enable dynamical menu 

endfunction
