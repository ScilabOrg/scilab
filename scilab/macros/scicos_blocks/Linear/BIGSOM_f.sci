//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=BIGSOM_f(job,arg1,arg2)
x=[];y=[];typ=[];
select job
case 'plot' then
  sgn=arg1.model.rpar
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
    [ok,sgn,exprs]=getvalue('Set sum block parameters',..
    			   'Inputs ports signs/gain',list('vec',-1),exprs)
    if ~ok then
      break
    end
    
    in = -ones(size(sgn,'*'),1)
    
    //** Patch: check added 
    [model,graphics,ok] = check_io(model,graphics,in,-1,[],[]);
    if ok then
      model.rpar = sgn(:)    ;
      graphics.exprs = exprs ;
      x.graphics = graphics;
      x.model = model ;
      break
    end
       
  end
case 'define' then
  sgn=[1;1]
  model=scicos_model()
  model.sim=list('sum',2)
  model.in=[-1;-1]
  model.out=-1
  model.rpar=sgn
  model.blocktype='c'
  model.dep_ut=[%t %f]

  
  exprs=sci2exp(sgn)
  gr_i=['[x,y,typ]=standard_inputs(o) ';
	'dd=sz(1)/8,de=0,'
	'if ~arg1.graphics.flip then dd=6*sz(1)/8,de=-sz(1)/8,end'
	'for k=1:size(x,''*'')';
	'  if sgn(k)>0 then';
	'    xstring(orig(1)+dd,y(k)-4,''+'')';
	'  else';
	'    xstring(orig(1)+dd,y(k)-4,''-'')';
	'  end';
	'end';
	'xx=sz(1)*[.8 .4 0.75 .4 .8]+orig(1)+de';
	'yy=sz(2)*[.8 .8 .5 .2 .2]+orig(2)';
	'xpoly(xx,yy,''lines'')']
  x=standard_define([2 3],model, exprs,gr_i)
end
endfunction
