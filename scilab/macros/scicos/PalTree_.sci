function PalTree_()
//** INRIA
//** Comments by Simone Mannori
  Cmenu = [] ;

  disablemenus();
    global scs_m_palettes ; //** potentially VERY dangerous 
    scs_m_palettes = do_pal_tree(scicos_pal); 
  enablemenus();  
endfunction
