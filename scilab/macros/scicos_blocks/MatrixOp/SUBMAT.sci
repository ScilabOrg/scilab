function [x,y,typ]=SUBMAT(job,arg1,arg2)
//
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1
  graphics=arg1.graphics;label=graphics.exprs
  model=arg1.model;
  if size(label,'*')==14 then label(9)=[],end //compatiblity
  while %t do
    [ok,typ,a,b,c,d,exprs]=getvalue('Set SUBMAT Block',..
	    ['Datatype (1=real double  2=Complex)';'Starting Row Index';'Ending Row Index';'Starting Column Index';'Ending Column Index'],list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),label)
    if ~ok then break,end
    if (typ==1) then
	junction_name='submat';
      	ot=1;
	it=1;
    elseif (typ==2) then
 	junction_name='submatz';
      	ot=2;
	it=2;
    else message("Datatype is not supported");ok=%f;
    end
    if (a<=0) | (b<=0) | (c<=0) | (d<=0) then message("invalid index");ok=%f;end
    model.ipar=[a;b;c;d];
    in=[model.in model.in2];
    out=[(b-a)+1 (d-c)+1];
    funtyp=4;
    if ok then
	label=exprs;
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      model.sim=list(junction_name,funtyp);
      graphics.exprs=label;
      arg1.graphics=graphics;arg1.model=model;
      x=arg1
      break
    end
  end
case 'define' then
  model=scicos_model()
  junction_name='submat';
  funtyp=4;
  model.sim=list(junction_name,funtyp)

  model.in=-1
  model.in2=-2
  model.intyp=1
  model.out=-1
  model.out2=-2
  model.outtyp=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[1;1;1;1]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[sci2exp(1);sci2exp(1);sci2exp(1);sci2exp(1);sci2exp(1)];
  gr_i=['xstringb(orig(1),orig(2),''SUBMAT'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

