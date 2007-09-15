function scilab2scicos(win,x,y,ibut)
  //utility function for the return to scicos by event handler
  if ibut==-1000|ibut==-1 then return,end
  ierr=execstr('load(TMPDIR+''/AllWindows'')','errcatch')
  if ierr==0 then
    for win_i= AllWindows'
      scf(win_i)
      seteventhandler('')
    end
  end
  scicos();
endfunction

