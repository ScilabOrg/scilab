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

function [x,y,typ]=PROD_f(job,arg1,arg2)
x=[];y=[];typ=[];
p=1 //pixel sizes ratio
select job
//**-----------------------------------------------------------------------
case 'plot' then
  wd = xget('wdim')
  graphics = arg1.graphics; 
  orig = graphics.orig,
  sz = graphics.sz
  orient = graphics.flip
  thick = xget('thickness');xset('thickness',2)
  //** patt=xget('dashes');xset('dashes',default_color(1))
  rx = sz(1)*p/2
  ry = sz(2)/2
  xarc(orig(1),orig(2)+sz(2),sz(1)*p,sz(2),0,23040) // (23040=360*64)
  gh_temp = gce(); 
  gh_temp.foreground = default_color(1);
  
  t=%pi/4
  xx=(orig(1)+rx)+..
      [sin(5*t) , sin(-t);
       sin(t) ,   sin(3*t)]*diag([rx;rx]/1.7)
  yy=(orig(2)+ry)+..
      [cos(5*t) , cos(-t);
       cos(t) ,   cos(3*t)]*diag([ry;ry]/1.7)
  xsegs(xx,yy,0)
  gh_temp = gce(); 
  gh_temp.segs_color = [default_color(1), default_color(1)] ;
  
  xset('thickness',1)
  if orient then  //standard orientation (port)
    out= [0  -1/14
	1/7    0
	0   1/14]*3
    xfpoly(sz(1)*out(:,1)+ones(3,1)*(orig(1)+sz(1)*p),sz(2)*out(:,2)+ones(3,1)*(orig(2)+sz(2)/2),1);
    gh_temp = gce(); 
    gh_temp.foreground = default_color(1);
    gh_temp.background = default_color(1);	
  else //tilded orientation
    out= [0   -1/14
	-1/7    0
	0   1/14]*3
    xfpoly(sz(1)*out(:,1)+ones(3,1)*orig(1),sz(2)*out(:,2)+ones(3,1)*(orig(2)+sz(2)/2),1);
    gh_temp = gce(); 
    gh_temp.foreground = default_color(1);
    gh_temp.background = default_color(1);
  end
    //** xset('dashes',patt)
//**-------------------------------------------------------------------------------------------
case 'getinputs' then
  graphics=arg1.graphics
  orig=graphics.orig
  sz=graphics.sz
  orient=graphics.flip

  wd=xget('wdim');
  if orient then
    t=[%pi -%pi/2]
  else
    t=[%pi %pi/2]
  end
  r=sz(2)/2
  rx=r*p
  x=(rx*sin(t)+(orig(1)+rx)*ones(t))
  y=r*cos(t)+(orig(2)+r)*ones(t)
  typ=ones(x)
  case 'getoutputs' then
  graphics=arg1.graphics; 
  orig=graphics.orig,
  sz=graphics.sz
  orient=graphics.flip
  wd=xget('wdim');
  if orient then
    t=%pi/2
    dx=sz(1)/7
  else
    t=-%pi/2
    dx=-sz(1)/7
  end
  r=sz(2)/2
  rx=r*p
  x=(rx*sin(t)+(orig(1)+rx)*ones(t))+dx
  y=r*cos(t)+(orig(2)+r)*ones(t)
  typ=ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim=list('prod',2)
  model.in=[-1;-1]
  model.out=-1
  model.blocktype='c'
  model.dep_ut=[%t %f]

  x=standard_define([1 1]/1.2,model,[],[])
end
endfunction
