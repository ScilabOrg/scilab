function [x,y,typ]=EXTRACTBITS(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  if (size(arg1.model.ipar,'*')~=4) then numb=[];
  else numb=arg1.model.ipar(3)
  end
  VOP=['Upper Half','Lower Half','Upper End','Lower End','      '+string(arg1.graphics.exprs(3))+'      ']
  OPER=VOP(evstr( arg1.graphics.exprs(2)))
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
    [ok,Datatype,rule,bit,scal,exprs]=getvalue('Set parameters',..
	['Datatype(3=int32 4=int16 5=int8 ...)';..
	 'Bits to extract(1=Upper Half 2=Lower Half 3=Range starting with most significant bit 4=Range ending with least significant bit 5=Range of bits)';..
	 'number of bits or index of bit (case range of bits:[start,end],0 is leat significant bit)';..
	 'Treat bit field as an integer(0=no 1=yes)'],..
	list('vec',1,'vec',1,'vec',-1,'vec',1),exprs)
    if ~ok then break,end
    if (rule<1)|(rule>5) then message('Incorrect index '+string(rule)+' ; must be 1 to 5.');ok=%f;end
    in=[model.in model.in2];bit=int(bit);rule=int(rule);
    if (or(bit(:)<0)) then
      message('Incorrect index '+string(min(bit))+' ; must be >0.');ok=%f;
    end
    if (rule==3)|(rule==4) then 
	if (size(bit,'*')~=1) then message('Index of bit must be a single value');ok=%f;
	else numb=bit;
	end
    elseif (rule==5)
        if (size(bit,'*')~=2) then message('Must have this form:[start,end]');ok=%f;
	else numb=bit(2)-bit(1);
	end ;
    else bit=0;numb=[]
    end
    if (Datatype==3) then
	if or(bit(:) > 31) then message ('Incorrect index '+string(max(bit))+' ; must be <32.');ok=%f;
	end
	 model.sim=list('extract_bit_i32',4)
    elseif (Datatype==4) then
	if or(bit(:) > 15) then message ('Incorrect index '+string(max(bit))+' ; must be <16.');ok=%f;
	end
	 model.sim=list('extract_bit_i16',4)
    elseif (Datatype==5) then
	if or(bit(:) > 7) then message ('Incorrect index '+string(max(bit))+' ; must be <8.');ok=%f;
	end
	 model.sim=list('extract_bit_i8',4)
    elseif(Datatype==6) then
	if or(bit(:) > 31) then message ('Incorrect index '+string(max(bit))+' ; must be <32.');ok=%f;
	end
	 model.sim=list('extract_bit_ui32',4)
    elseif (Datatype==7) then
	if or(bit(:) > 15) then message ('Incorrect index '+string(max(bit))+' ; must be <16.');ok=%f;
	end
	 model.sim=list('extract_bit_ui16',4)
    elseif (Datatype==8) then
	if or(bit(:) > 7) then message ('Incorrect index '+string(max(bit))+' ; must be <8.');ok=%f;
	end
	 model.sim=list('extract_bit_ui8',4)
    else message ('Datatype '+string(Datatype)+' is not supported ; It must be 3 to 8');ok=%f;
    end
      if ok then
	it=Datatype
	ot=Datatype
	out=[1 1]
	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      end
      if ok then
	graphics.exprs=exprs;
	model.ipar=[int(rule-1);int(scal);int(bit(:));int(numb(:))];
	x.graphics=graphics;x.model=model;
	break
      end
  end

case 'define' then
  numb=[];
  model=scicos_model()
  model.sim=list('extract_bit',4)
  model.in=1
  model.in2=1
  model.out=1
  model.out2=1;
  model.intyp=3
  model.outtyp=3
  model.ipar=[0,0,numb]
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[sci2exp(3);sci2exp(1);sci2exp(0);sci2exp(0)]
  gr_i=['xstringb(orig(1),orig(2),['' Extract ''+string(numb)+'' Bits'';OPER],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
  
end
endfunction
