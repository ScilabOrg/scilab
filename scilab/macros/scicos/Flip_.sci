function Flip_()
// 29 Jun 2006 :)
//** Alan/Simone 13/12/06 : Replot no more needed
//** August 2007 : first version with connected blocks
//** 
//*
//** BEWARE: this version has some residual graphical bug with some 
//**         connected blocks
//*    
   scs_m_save = scs_m    ; //** save the old diagram for "undo" operation 
   nc_save = needcompile ; //** save the old diagram state
   
   [%pt, scs_m] = do_tild(%pt, scs_m) ; //** do the "tilt"/"flip" operation 
   
   Cmenu = []; //** 
   %pt   = [];
endfunction
