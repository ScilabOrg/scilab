function PalTree_()
  Cmenu=[]
  disablemenus()
  global scs_m_palettes
  scs_m_palettes=do_pal_tree(scicos_pal);
  enablemenus() 
endfunction
