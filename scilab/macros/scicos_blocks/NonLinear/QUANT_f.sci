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

function [x,y,typ]=QUANT_f(job,arg1,arg2)
x=[];y=[],typ=[];
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
  while %t then
    [ok,pas,meth,exprs]=getvalue('Set parameters',..
	['Step';'Quantization Type (1-4)'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if meth<1|meth>4 then
      message('Quantization Type must be from 1 to 4')
    else
      rpar=pas
      model.rpar=rpar
      model.ipar=meth
      select meth
      case 1 then
	model.sim='qzrnd'
      case 2 then
	model.sim='qztrn'
      case 3 then
	model.sim='qzflr'
      case 4  then
	model.sim='qzcel'
      end
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  pas=0.1;
  meth=1
  model=scicos_model()
  model.sim='qzrnd'
  model.in=-1
  model.out=-1
  model.rpar=pas
  model.ipar=meth
  model.blocktype='c'
  model.dep_ut=[%t %f]

  exprs=[string(pas);string(meth)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[1;2;2;3;3;4;4]/5*sz(1);';
    'yy= orig(2)+[1;1;2;2;3;3;4]/5*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
