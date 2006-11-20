function Context_()
    Cmenu='Replot'
    while %t do
      //** %now_win=xget('window')
      gh_now_win = gcf() ;
      %now_win = gh_now_win.figure_id ;
      [context,ok]=do_context(scs_m.props.context)
      //** xset('window',%now_win)
      scf(%now_win) ;
      if ~ok then break,end
      clear %scicos_context  // to make sure only parent context is used
      if ~exists('%scicos_context') then
	%scicos_context=struct()
      end
      disablemenus()
      [%scicos_context,ierr]=script2var(context,%scicos_context)
      //for backward compatibility for scifunc
      if ierr==0 then
	%mm=getfield(1,%scicos_context)
	for %mi=%mm(3:$)
	  ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
	  if ierr<>0 then
	    break
	  end
	end
      end
      //end of for backward compatibility for scifunc
      enablemenus()
      if ierr <>0 then
	message(['Error occur when evaluating context:'
		 lasterror() ])
      else
	scs_m.props.context=context;
	disablemenus()
	do_terminate()
	[scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
	if needcompile<>4&size(%cpr)>0 then %state0=%cpr.state,end
	edited=%t
	alreadyran=%f
	enablemenus()
	break,
      end
    end
endfunction



