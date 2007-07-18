function New_()
//** INRIA 
//**     
    Cmenu = [] ; //** clear previous command 
    
    %r = 1     ; //** preload the default answer
    if edited then
      %r = message(["Diagram has not been saved";
	            "Are you sure you want to proceed ?"],['No';'Yes'])
    end             //** No -> %r = 1 ; Yes -> %r = 2  
    
    if %r==2 then
      if alreadyran then
          do_terminate() ; // terminate current simulation
      end  
      
      alreadyran = %f ; 
      scs_m = scicos_diagram(version=get_scicos_version()) ; //** create a empty diagram 
      %wpar = scs_m.props ; 
      %zoom  = 1.4        ; //** original value by Ramine
      Cmenu  = "Replot"   ; //** force a Replot of the New (empty) diagram
      edited = %f         ; //** not edited    
    end
endfunction
