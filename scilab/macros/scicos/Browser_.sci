function Browser_()
//** INRIA
//** Comments by Simone Mannori

Cmenu = [] ; 

if super_path==[] then
  disablemenus();
   do_browser(scs_m);
  enablemenus();  
else        
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"Browser'";%scicos_navig=[]';
		       '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		      ]  // not really needed to return anymore, may remove last line
//  message("Browser can only be launched from main diagram.") ;
end

endfunction
