function do_browser(scs_m)
//** INRIA
//** Comments by Simone Mannori

  tt = scs_TreeView(scs_m);
  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(tt,scs_m.props.title(1)+'.tcl');
  chdir(cur_wd)
  TCL_EvalFile(TMPDIR+'/'+scs_m.props.title(1)+'.tcl')
endfunction

function tt = scs_TreeView(scs_m)
  x = [];
  y = 0 ;
tt=["package require BWidget 1.7.0"
    'set w .scsTree'
    'proc ppsc {label} {global blkox; set blkox $label;ScilabEval '"Cmenu=''BrowseTo'''"}'
    'catch {destroy $w}'
    'toplevel $w'
    'Tree $w.t -xscrollcommand {$w.xsb set} -yscrollcommand {$w.ysb set} "+...
      " -width 50 -bg white'
    'scrollbar $w.ysb -command {$w.t yview}'
    'scrollbar $w.xsb -command {$w.t xview} -orient horizontal'
    'grid $w.t $w.ysb -sticky nsew'
    ' grid $w.xsb -sticky ew'
    ' grid rowconfig    $w 0 -weight 1'
    ' grid columnconfig $w 0 -weight 1'
   ];

tt = [tt;'wm title $w {Browser (double click to open diagram)}'];
Path = 'root'
tt = crlist1(scs_m,Path,tt);

tt = [tt;' $w.t bindText <Double-1> {ppsc}'];

endfunction
 
//TCL_EvalStr('$w.t opentree node1')
//   pa=TCL_GetVar('x');pa=part(pa,6:length(pa));
//   execstr('pa=list('+pa+')');       o=scs_m(scs_full_path(pa))


function tt = crlist1(scs_m,Path,tt)
  for i=1:size(scs_m.objs)
    o = scs_m.objs(i);
    
    if typeof(o)=="Block" then
      path = Path+','+string(i)
      
      if o.model.sim=='super' then
	titre2 = o.model.rpar.props.title(1);
	tt = [tt;'$w.t insert end '+Path+' '+path+' -text '"'+titre2+''"']

	tt = crlist1(o.model.rpar,path,tt); //** BEWARE: Recursive Call at the 
                                            //** very same function 
      end 
    
    end //** Blocks and Super Blocks filter 
  
   end //**..  loop on objects 
endfunction

