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

function [x,y,typ]=DEMUX(job,arg1,arg2)
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
      [ok,out,exprs]=scicos_getvalue('Set DEMUX block parameters',..
			      ['number of output ports or vector of sizes'],list('vec',-1),exprs)
      if ~ok then break,end
      if size(out,'*')==1 then
	if out<2|out>31 then
	  message('Block must have at least 2 and at most 31 output ports')
	  ok=%f
	else
	  [model,graphics,ok]=check_io(model,graphics,0,-[1:out]',[],[])
	end
      else
        if size(out,'*')<2| or(out==0)|size(out,'*')>31 then
	  message(['Block must have at least 2 and at most 31 output ports';
		   'size 0 is not allowed'])
	  ok=%f
	else
	  if min(out)<0 then nin=0,else nin=sum(out),end
	  [model,graphics,ok]=check_io(model,graphics,nin,out(:),[],[])
	  if ok then out=size(out,'*'),end
	end
      end
      if ok then
	graphics.exprs=exprs;model.ipar=out
	x.graphics=graphics;x.model=model
	break
      end
    end
   case 'define' then
    out=2
    model=scicos_model()
    model.sim=list('multiplex',4)
    model.in=0 //means equal to the sum of the outputs
    model.out=-[1:out]'
    model.ipar=out
    model.blocktype='c'
    model.firing=[]
    model.dep_ut=[%t %f]

    exprs=string(out)
    gr_i=['txt=''Demux'';'
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
