function ScicosDocumentation_()
//** INRIA
//** 14/09/07 Alan

  if execstr('help(''whatis_scicos'');' , 'errcatch') <>0 then
     mess = "No help available.";
  end

  //** Clear state machine variables
  Cmenu = [];

endfunction
