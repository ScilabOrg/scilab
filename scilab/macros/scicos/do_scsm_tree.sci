function scs_m = do_scsm_tree(scs_m)
  tt=scs_m_TreeView(scs_m);
  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(tt,scs_m.props.title(1)+'.tcl');
  chdir(cur_wd)
  TCL_EvalFile(TMPDIR+'/'+scs_m.props.title(1)+'.tcl')
endfunction

function tt=scs_m_TreeView(scs_m)
  x=[];y=0;
tt=['package require BWidget'
    'set w .palettes'
    'proc ppxs {label} {global blko; set blko $label;ScilabEval '"Cmenu=''PlaceinDiagram'''"}'
    'proc qqxs {label} {global blko; set blko $label;ScilabEval '"Cmenu=''TkPopup'''"}'
    'catch {destroy $w}'
    'toplevel $w'
    'Tree $w.t -xscrollcommand {$w.xsb set} -yscrollcommand {$w.ysb set} "+...
    "-deltax 10 -deltay 10 -width 30 -bg white'
    'scrollbar $w.ysb -command {$w.t yview}'
    'scrollbar $w.xsb -command {$w.t xview} -orient horizontal'
    'grid $w.t $w.ysb -sticky nsew'
    ' grid $w.xsb -sticky ew'
    ' grid rowconfig    $w 0 -weight 1'
    ' grid columnconfig $w 0 -weight 1'
   ];

tt=[tt;'wm title $w '"Navigator:'" '+scs_m.props.title(1)];

Path='root'
tt=crlist2(scs_m,Path,tt);

tt=[tt;' $w.t bindText <Double-1> {ppxs}'];
tt=[tt;' $w.t bindText <3> {qqxs}'];
endfunction
 
//TCL_EvalStr('$w.t opentree node1')
//   pa=TCL_GetVar('x');pa=part(pa,6:length(pa));
//   execstr('pa=list('+pa+')');       o=scs_m(scs_full_path(pa))


function tt=crlist2(scs_m,Path,tt)
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i);
    path=Path+','+string(i)
    if typeof(o)=='Link' then
      titre2='link'
      tt=[tt;'$w.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    elseif typeof(o)=='Deleted' then
      titre2='Deleted'
      tt=[tt;'$w.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    else
      if o.model.sim=='super' then
	titre2=o.model.rpar.props.title(1);
	tt=[tt;'$w.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text '"'+titre2+''"']
	tt=crlist2(o.model.rpar,path,tt)
      else
	titre2=o.gui;
	  tt=[tt;'$w.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
      end
    end
  end
  
endfunction

