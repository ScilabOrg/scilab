function [x,y,typ]=SELECT_m(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,typ,nin,z0,exprs]=getvalue('Set parameters',..
	['Datatype(1= real double  2=Complex)';'number of inputs';'initial connected input'],..
	list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if z0>nin|z0<=0 then
      message('initial connected input is not a valid input port number')
    else
      if typ==1 then
      junction_name='selector_m'
      it=ones(1,nin)
      ot=1
      elseif typ==2 then
      junction_name='selector_zm'
      it=2*ones(1,nin)
      ot=2
      else message("Datatype is not supported");ok=%f;end
      if ok then
      in=[-ones(nin,1) -2*ones(nin,1)]
      out=[-1 -2]
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(nin,1),[])
      if ok then
        model.sim=list(junction_name,4)
	graphics.exprs=exprs;
	model.dstate=z0,
	x.graphics=graphics;x.model=model
	break
      end
      end
    end
  end
case 'define' then
  z0=1
  nin=2

  model=scicos_model()
  model.sim=list('selector_m',4)
  model.in=[-1;-1]
  model.in2=[-2;-2]
  model.intyp=1
  model.out=-1
  model.out2=-2
  model.outtyp=1
  model.evtout=[]
  model.state=[]
  model.rpar=[]
  model.ipar=[] 
  model.firing=[]
  model.evtin=ones(nin,1)
  model.dstate=z0
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[sci2exp(1);sci2exp(nin);sci2exp(z0)]
  gr_i=['xstringb(orig(1),orig(2),''Selector'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
