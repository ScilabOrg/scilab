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

function [x,y,typ]=AFFICH_f(job,arg1,arg2)
//** 22 Aug 2006: looking for a nasty "font" bugs 
x=[];y=[];typ=[]

select job

case 'plot' then
  ipar = arg1.model.ipar
  standard_draw(arg1)

case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)

case 'getoutputs' then
  x=[];y=[];typ=[];

case 'getorigin' then
  [x,y]=standard_origin(arg1)

case 'set' then
  x = arg1;
  graphics = arg1.graphics; exprs = graphics.exprs
  model = arg1.model;
  if size(exprs,'*')==5 then exprs(6)='0';end //compatibility
  if size(exprs,'*')==4 then exprs(4)=' ';exprs(5)=' ';end //compatibility
  if size(model.dstate,'*')<>6 then model.dstate=[0;-1;0;0;1;1];end //compatibility
  while %t do
    [ok,font,fontsize,colr,nt,nd,herit,exprs]=getvalue(..
	'Set  parameters',..
	['Font number';
	 'Font size';
	 'Color';
	 'Total number of digits';
	 'Number of rational part digits';
	 'Block inherits (1) or not (0)'],..
	 list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs)
    
    if ~ok then break,end //user cancel modification
    
    
    //** ------------------------ Error checking ----------------------------------
    mess = [] ; //** no message

    if font<=0 then
      mess=[mess;'Font number must be positive';' ']
      ok=%f
    end
    
    if fontsize<=0 then
      mess=[mess;'Font size must be positive';' ']
      ok=%f
    end
    
    if nt<=3 then
      mess=[mess;'Total number of digits must be greater than 3';' ']
      ok=%f
    end
    
    if nd<0 then
      mess=[mess;'Number of rational part digits must be '
	  'greater or equal 0';' ']
      ok=%f
    end
    
    if ~ok then
      message(['Some specified values are inconsistent:';
	' ';mess]);
    end
	   
    if ~or(herit==[0 1]) then
      mess=[mess;'Accept inherited values are 0 and 1';' ']
      ok=%f
    end
    
    if ~ok then
      message(['Some specified values are inconsistent:';
	  ' ';mess])
    end
    //**---------------------------------------------------------------------
    
    //** Positive case ->
    if ok then
      [model,graphics,ok]=check_io(model,graphics,1,[],ones(1-herit,1),[])
    end
    
    if ok then
      model.ipar=[font;fontsize;colr;xget('window');nt;nd];
      model.evtin=ones(1-herit,1)
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
    
  end


case 'define' then
  font = 1     ; //** 1 --> greek font 
  fontsize = 1 ; //** 1
  colr = 1 ;
  nt = 9   ;
  nd = 2   ;
  exprs = [ string(font);
            string(fontsize);
            string(colr);
            string(nt);
            string(nd);
	    string(0) ]
	    
  model = scicos_model(); 
  model.sim = 'affich' ; 
  model.in = 1 ;
  model.evtin  = 1 ;
  model.dstate = [0;-1;0;0;1;1]
  model.ipar   = [font;fontsize;colr;1000;nt;nd]
  model.blocktype = 'c' ;
  model.firing = []     ;
  model.dep_ut = [%t %f]
  model.label = '' ;
  
  gr_i=['xset(''font'',ipar(1),ipar(2))';
	'str='' ''+part(''0'',ones(1,ipar(5)-ipar(6)-2))+''.''+part(''0'',ones(1,ipar(6)))'
	'rr=xstringl(orig(1),orig(2),str)'
        'xstring(orig(1)+max(0,(sz(1)-rr(3))/2),orig(2)+max(0,(sz(2)-rr(4))/2),str)']
  
  // disp("AFFICH_f gr_i"); pause ; //** debug only  
	
  x = standard_define([3 2],model,exprs,gr_i)

end

endfunction
