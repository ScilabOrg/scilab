function New_()
//** INRIA 
//**     
    Cmenu = [] ;
    %r = 2 ;
  
    if edited then
      %r = message(['Diagram has not been saved';
	  'Are you sure you want to proceed'],['No';'Yes'])
    end
    
    if %r==2 then
      if alreadyran then
          do_terminate() ; //terminate current simulation
      end  
      
      alreadyran = %f ; 
      scs_m = scicos_diagram(version=get_scicos_version()) ; //** create a empty diagram 
      %wpar = scs_m.props ; 
      //** window_set_size()   ; //** resize the phisical window  
      Cmenu  = 'Replot'   ; //** force a Replot of the New (empty) diagram
      edited = %f         ; //** not edited    
    end
endfunction
