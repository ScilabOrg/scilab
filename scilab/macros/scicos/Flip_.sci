function Flip_()
// 29 Jun 2006 :)
//** Alan/Simone 13/12/06 : Replot no more needed
   scs_m_save = scs_m ;
   nc_save = needcompile;
   [%pt,scs_m] = do_tild(%pt,scs_m)
   Cmenu=[];
   %pt = [];
endfunction
