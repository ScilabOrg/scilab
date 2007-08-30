function do_exit()
// Copyright INRIA

//** This function is executed at the end of the SCICOS main loop
//
//** N.B. In case of "super_block" the 'scicos()' is resursively reactivate 
//**      inside the 'set' section of 'SUPER_f.sci' interfacing function of
//**      Super Block 

//** xinfo(" "); 

r = 0 ; 

//**---------------------------------------------------------------
//** Exit from scicos top diagram edited: ask for save the diagram 
if edited then

  if ~super_block then
    r = x_choose(['Save';'Save As'],..
	         ['Diagram has not been saved';
	          'What to do before leaving?'],'Don''t save')
  end

end

//** if the answer is positive 
if r==1 then
  ok = do_save(); //** save the diagram 
  if ~ok then do_SaveAs();end; //** or ask to "Save As"
elseif r==2 then //** ask to Save As
  do_SaveAs()
end

//**---------------------------------------------------------------

 
//** If is not a superblock and is NOT a palette  
//** if ~super_block & ~pal_mode  then

//**----------------------------------------------------------------
if ~super_block then //** if I'm exit from the main Scicos diagram
  //** if the simulation is not finished call 'do_terminate()' housekeeping routine 
  if alreadyran then do_terminate(),end ; //** terminate SCICOS simulation 

end
//**----------------------------------------------------------------

//**--------------------------------------------------------------
//** close all the unuseful window 
for win=windows(size(windows,1):-1:noldwin+1,2)'
  
  if or(win==winsid()) then
    gh_del = scf(win) ; //** select the 'win'window and get the handle
    delete (gh_del)   ; //** delete the window   
  end

end

endfunction
