function UpToMainDiagram_()
  Cmenu=[]
  if super_block then
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"[]'";%scicos_navig=[]'
		      ]
  else
      message('This is already the main diagram;')
  end
endfunction