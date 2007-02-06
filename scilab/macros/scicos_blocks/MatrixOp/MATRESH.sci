function [x,y,typ]=MATRESH(job,arg1,arg2)
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
  model=arg1.model;graphics=arg1.graphics;label=graphics.exprs
  if size(label,'*')==14 then label(9)=[],end //compatiblity
  while %t do
    [ok,typ,l1,out,lab]=..
        getvalue('Set MATRESH block parameters',..
        ['Datatype(1=real double  2=Complex)'
	 'input size';
         'output size desired'],..
         list('vec',-1,'vec',-1,'vec',-1),label)
    if ~ok then break,end
    nout=size(out)
    nin=size(l1)
    if nout==0 then
      message('output must have at least one element');ok=%f;
    end
    if nin==0 then
      message('input must have at least one element');ok=%f;
    end
    if ok then
    if ((out(1)>(l1(1)*l1(2)))) then
	message("the first dimension of the output is too big");ok=%f;
    end
    if ((out(2)>(l1(1)*l1(2)))) then
	message("the second dimension of the output is too big");ok=%f;
    end
    if (((out(2)*out(1))>(l1(1)*l1(2)))) then
	message("the dimensions of the output are too big");ok=%f;
    end
    end
     if (typ==1) then
	junction_name='mat_reshape';
      	ot=1;
	it=1;
    elseif (typ==2) then
 	junction_name='matz_reshape';
      	ot=2;
	it=2;
    else message("Datatype is not supported");ok=%f;
    end
    if ok then
      label=lab
      [model,graphics,ok]=set_io(model,graphics,list(l1,it),list(out,ot),[],[])
    end
    if ok then
      funtyp=4;
      model.sim=list(junction_name,funtyp)
      graphics.exprs=label
      arg1.graphics=graphics
      arg1.model=model
      x=arg1
      break
    end
  end
  needcompile=resume(needcompile)
case 'define' then
  model=scicos_model()
  junction_name='mat_reshape';
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
  model.ipar=[]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[sci2exp(1);sci2exp([]);sci2exp([])];
  gr_i=['xstringb(orig(1),orig(2),''RESHAPE'',sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label,gr_i)
end
endfunction

