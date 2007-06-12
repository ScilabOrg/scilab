function [x,y,typ]=INTLOGICAL_OP(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  VOP=['AND', 'OR', 'NAND', 'NOR', 'XOR','NOT']
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,Datatype,nin,rule,typ,exprs]=getvalue('Set parameters',..
	['Datatype (3=int32 4=int16 5=int8 ...)';..
	 'number of inputs';..
	 'Operator: AND (0), OR (1), NAND (2), NOR (3), XOR (4), NOT (5)';..
	 'Bitwise Rule (0=No 1=Yes)'],..
	list('vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    nin=int(nin);rule=int(rule);
    if nin<1 then
      message('Number of inputs must be >=1 ');ok=%f;
    elseif (rule<0)|(rule>5) then
      message('Incorrect operator '+string(rule)+' ; must be 0 to 5.');ok=%f;
    elseif (rule==5)&(nin>1) then 
      message('Only one input allowed for NOT operation')
      nin=1
    end
    if ok then
      if (typ~=0) then typ=1; end
      if Datatype==3 then 
	model.sim=list('logicalop_i32',4)
      elseif Datatype==4 then
	model.sim=list('logicalop_i16',4)
      elseif Datatype==5 then
	model.sim=list('logicalop_i8',4)
      elseif Datatype==6 then
	model.sim=list('logicalop_ui32',4)
      elseif Datatype==7 then
	model.sim=list('logicalop_ui16',4)
      elseif Datatype==8 then
	model.sim=list('logicalop_ui8',4)
      else message ("Datatype is not supported");ok=%f;end
      if ok then
	it=Datatype*ones(nin,1);
	ot=Datatype;
	in=[-ones(nin,1) -2*ones(nin,1)]
      	if (rule<>5)&(nin==1) then
	    out=[1 1]
	    [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
        else
	    out=[-1 -2]
	    [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
        end
      end
      if ok then
	graphics.exprs=exprs;
	model.ipar=[rule;typ],
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  in=[-1;-1]
  ipar=[0;0]
  nin=2
  
  model=scicos_model()
  model.sim=list('logicalop_i32',4)
  model.in=in
  model.in2=[-2;-2]
  model.out=-1
  model.out2=-2;
  model.intyp=[3 3]
  model.outtyp=3
  model.ipar=ipar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[sci2exp(3);string(nin);string(ipar(1));string(ipar(2))]
  gr_i=['xstringb(orig(1),orig(2),['' INTLogical Op '';OPER],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
