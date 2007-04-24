function AboutSCICOS_()
//   INRIA
//** 21 Nov 2006
//** Comments by Simone Mannori

    Cmenu=[] ; %pt=[];

    disablemenus() ; //** disable dynamical menu (just to be safe)

    x_message([strsubst(get_scicos_version(),"scicos","SCICOS -")+"-"]);

    enablemenus() ;  //** re-enable dynamical menu

endfunction
