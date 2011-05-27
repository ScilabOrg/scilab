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

function [x,y,typ]=MUX(job,arg1,arg2)
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
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,in,exprs]=scicos_getvalue('Set MUX block parameters',..
	'number of input ports or vector of sizes',list('intvec',-1),exprs)
    if ~ok then break,end
    if size(in,'*')==1 then
      if in<2|in>31 then
	message('Block must have at least two input ports and at most 31')
	ok=%f
      else
	[model,graphics,ok]=check_io(model,graphics,-[1:in]',0,[],[])
      end
    else
      if size(in,'*')<2| or(in==0)|size(in,'*')>31 then
	message(['Block must have at least two input ports';
		 'and at most 31. Size 0 is not allowed. '])
	ok=%f
      else
	if min(in)<0 then nout=0,else nout=sum(in),end
	[model,graphics,ok]=check_io(model,graphics,in(:),nout,[],[])
	if ok then in=size(in,'*'),end
      end
    end
    if ok then
      graphics.exprs=exprs;model.ipar=in
      x.graphics=graphics;x.model=model
      break
    end
  end
 case 'define' then
  in=2
  model=scicos_model()
  model.sim=list('multiplex',4)
  model.in=-[1:in]'
  model.out=0
  model.ipar=in
  model.blocktype='c'
  model.dep_ut=[%t %f]

  exprs=string(in)
  gr_i=['txt=''Mux'';'
        'style=5;'
        'rectstr=stringbox(txt,orig(1),orig(2),0,style,1);'
        'if ~exists(''%zoom'') then %zoom=1, end;'
        'w=(rectstr(1,3)-rectstr(1,2))*%zoom;'
        'h=(rectstr(2,2)-rectstr(2,4))*%zoom;'
        'xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');'
        'e=gce();'
        'e.font_style=style;']
  x=standard_define([.5 2],model,exprs,gr_i)
end
endfunction
