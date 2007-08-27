function tree_show(x)
 if type(x)<>16&type(x)<>17&type(x)<>15 then error('Wrong type; input must be a list.');end
tt=['package require BWidget'
'proc ppx {label} {global xmind; set xmind $label;ScilabEval '"%tcl_par=''1'''"}'
'proc qqx {label} {global xmind; set xmind $label;ScilabEval '"%tcl_par=''3'''"}'
'catch {destroy .ss}'
'toplevel .ss'
'Tree .ss.t -xscrollcommand {.ss.xsb set} -yscrollcommand {.ss.ysb set} "+...
    " -width 30'
'scrollbar .ss.ysb -command {.ss.t yview}'
'scrollbar .ss.xsb -command {.ss.t xview} -orient horizontal'
'grid .ss.t .ss.ysb -sticky nsew'
' grid .ss.xsb -sticky ew'
' grid rowconfig    .ss 0 -weight 1'
' grid columnconfig .ss 0 -weight 1'
];
for i=1:size(tt,1)
  TCL_EvalStr(tt(i))
end

v=getfield(1,x);

tt='wm title .ss '+v(1);
TCL_EvalStr(tt)
Path='root'
crlist(x,Path);

TCL_EvalStr(' .ss.t bindText <Double-1> {ppx}')
TCL_EvalStr(' .ss.t bindText <3> {qqx}')
endfunction


function crlist(x,Path)
  if type(x)==15 then
      II=1:size(x);v=string(II);
  else 
      v=getfield(1,x);II=2:size(v,'*');
  end
  for i=II
    path=Path+','+string(i)
    titre=v(i);
    o=getfield(i,x);
    if type(o)==16 |type(o)==17 then
        w=getfield(1,o);
	titre2=titre+' ('+w(1)+')';
	TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text {'+titre2+'}')
	crlist(o,path)
    elseif type(o)==15 then
	titre2=titre;
	TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text {'+titre2+'}')
	crlist(o,path)
    else
        titre2=titre+': '+sprintf('%s',sci2exp(o))  ;
        TCL_EvalStr('set yy {'+titre2+'}')
        TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -text $yy')
    end
  end
  
endfunction

