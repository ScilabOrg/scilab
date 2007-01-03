function Grid_()
//** INRIA
  Cmenu=[]
  %scicos_with_grid(1)=~%scicos_with_grid(1);
  gh_window = gcf(%win);

  if %scicos_with_grid then
    gh_window.children.children($).visible="on"
  else
    gh_window.children.children($).visible="off"
  end
  drawnow
  show_pixmap
endfunction