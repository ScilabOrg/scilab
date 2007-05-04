function [x,y,typ]=RELATIONAL_OP(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  VOP=['==', '~=', '<', '<=', '>','>=']
  OPER=VOP(evstr( arg1.graphics.exprs(2))+1)
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
    [ok,Datatype,rule,zcr,exprs]=getvalue('Set parameters',..
	['Datatype( 3=int32 4=int16 5=int8 ...)';..
	 'Operator: == (0), ~= (1), < (2), <= (3), > (4), >= (5)';..
	 'Zero crossing (0=no 1=yes)'],..
	list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    in=[model.in model.in2];rule=int(rule);if zcr<>0 then zcr=1,end
    if (rule<0)|(rule>5) then
      message('Incorrect operator '+string(rule)+' ; must be 0 to 5.');ok=%f;
    end
    if (Datatype==3) then
        model.sim=list('relational_op_i32',4)
    elseif(Datatype==4) then
        model.sim=list('relational_op_i16',4)
    elseif(Datatype==5) then
        model.sim=list('relational_op_i8',4)
    elseif(Datatype==6) then
        model.sim=list('relational_op_ui32',4)
    elseif(Datatype==7) then
        model.sim=list('relational_op_ui16',4)
    elseif(Datatype==8) then
        model.sim=list('relational_op_ui8',4)
    else message("Datatype is not supported; must be 3 to 8");ok=%f;
    end
      if ok then
	it=Datatype*ones(1,2)
	ot=Datatype
	out=[-1 -2]
	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      end
      if ok then
	graphics.exprs=exprs;
	model.ipar=[rule];
	model.nzcross=zcr;
	model.nmode=zcr;
	x.graphics=graphics;x.model=model;
	break
      end
  end

case 'define' then
  model=scicos_model()
  model.sim=list('relational_op_i32',4)
  model.in=[-1;-1]
  model.in2=[-2;-2]
  model.out=-1
  model.out2=-2;
  model.intyp=[3 3]
  model.outtyp=3
  model.ipar=0
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[sci2exp(3);sci2exp(0);sci2exp(0)]
  txt=['Relational';'Operator']
  gr_i=['xstringb(orig(1),orig(2),[OPER],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.id=txt
end
endfunction
