function [x,y,typ]=SHIFT(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  sgn=arg1.model.ipar
  VOP=["Arithmetic", "Circular"]
  OPER=VOP(evstr( arg1.graphics.exprs(3))+1)
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics
  model=arg1.model
  exprs=graphics.exprs
  while %t do
    [ok,Datatype,nb,np,exprs]=getvalue('Set Shift block parameters',..
			    ['Datatype (3=int32  4=int16 5=int8 ...)';..
                             'Number of bits to shift left (use negatif number to shift right)';..
                             'Shifttype(0=Arithmetic 1=Circular)'],..
                             list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if (np~=0 & np~=1) then message ("shifttyp is not supported");ok=%f;end
    it=Datatype;
    ot=Datatype;
//    model.sim=list('shift_ia',4)
    if Datatype==3 then
        model.sim=list('shift_i32',4)
    elseif Datatype==4 then
        model.sim=list('shift_i16',4)
    elseif Datatype==5 then
      	model.sim=list('shift_i8',4)
    elseif Datatype==6 then
       	model.sim=list('shift_i32',4)
    elseif Datatype==7 then
        model.sim=list('shift_i16',4)
    elseif Datatype==8 then
        model.sim=list('shift_i8',4)
    else message("Datatype is not supported");ok=%f;
    end
    if ok then
      [model,graphics,ok]=set_io(model,graphics,...
                                 list([-1,-2],it),...
                                 list([-1,-2],ot),[],[])
    end
    if ok then
      model.ipar=[nb,np]
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  sgn=[0;0]
  model=scicos_model()
  model.sim=list('shift_i32',4)
  model.in=-1
  model.out=-1
  model.in2=-2
  model.out2=-2
  model.intyp=3
  model.outtyp=3
  model.rpar=[]
  model.ipar=sgn
  model.blocktype='c'
  model.dep_ut=[%t %f]

  
  exprs=[sci2exp(3);sci2exp(0);sci2exp(0)]
gr_i=['xstringb(orig(1),orig(2),[OPER;''   Shift  ''],sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model, exprs,gr_i)
end
endfunction
