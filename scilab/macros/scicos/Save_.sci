function Save_()
//** INRIA
//** 
    Cmenu = [] ;
    %pt = []   ;
    
    ok = do_save(scs_m) ; 
     
    if ok & ~super_block then
       edited = %f ;
    end
endfunction
