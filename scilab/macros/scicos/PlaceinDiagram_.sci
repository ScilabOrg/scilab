function PlaceinDiagram_()
  
  Cmenu=[]
  disablemenus()
  global scs_m_palettes
  %pa_=TCL_GetVar('blko');%pa_=part(%pa_,6:length(%pa_));execstr('%pa_=list('+%pa_+')');
  scs_m=do_placeindiagram(scs_m,scs_m_palettes(scs_full_path(%pa_)))
  enablemenus() ;
endfunction
