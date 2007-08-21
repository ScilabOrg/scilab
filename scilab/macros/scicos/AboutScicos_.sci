function AboutScicos_()
//   INRIA
//** 21 Nov 2006
//** Comments by Simone Mannori

    Cmenu=[] ; %pt=[];

    disablemenus() ; //** disable dynamical menu (just to be safe)

    message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "Do you want to see more ?";...
	     "     www.scicos.org      "]);

    enablemenus() ;  //** re-enable dynamical menu

endfunction
