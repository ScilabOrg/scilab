function [x,y,typ]=CMATVIEW(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(o)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,win,nom,colormap,cmin,cmax,exprs]=getvalue(..
	'Set Scope parameters',..
	['Output window number';
	'Name of Scope (label&Id)';
	'ColorMap';
	'Minimum level range';
	'Maximum level range'],..
	 list('vec',-1,'str',1,'vec',-1,'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification
    mess=[]
    if win<-1 then
      mess=[mess;'Window number can''t be  < -1';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';' ';mess])
    end
    if ok then
	size_c=size(colormap(:),1);
        sol=inv([cmin 1;cmax 1])*[1;size_c/3];
        alpha_c = sol(1);
        beta_c = sol(2);
	ipar=[win;cmin;cmax;size_c];
  	rpar=[alpha_c;beta_c;colormap(:)];
	model.ipar=ipar;
	model.rpar=rpar;
	model.label=nom;
	graphics.id=nom;
	graphics.exprs=exprs;
	x.graphics=graphics;
        x.model=model
	break
    end
  end
case 'define' then
  win=-1;
  cmin = 0;
  cmax = 100;
  colormap = jetcolormap(25);
  size_c = 25;
  sol=inv([cmin 1;cmax 1])*[1;size_c];
  alpha_c = sol(1);
  beta_c = sol(2);

  model=scicos_model()
  model.sim=list('cmatview',4)
  model.in=-1
  model.in2=-2
  model.intyp=1
  model.evtin=1
  model.ipar=[win;cmin;cmax;size_c]
  model.rpar=[alpha_c;beta_c;colormap(:)]
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(win);
	 emptystr();
	 string('jetcolormap(25)');
	 string(cmin);
	 string(cmax)];
   gr_i='xstringb(orig(1),orig(2),''Mat. View'',sz(1),sz(2),''fill'')'
   x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
