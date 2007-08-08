function [x,y,typ]=SampleCLK(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    pat=xget('pattern'); xset('pattern',default_color(0))
    orig=arg1.graphics.orig;
    sz=arg1.graphics.sz;
    orient=arg1.graphics.flip;
    xarcs([orig(1),orig(2)+sz(2),sz(1),sz(2),-360*64,360*64]',5);
    x=orig(1)*ones(1,2)+sz(1)*[1/2 1/2];
    y=orig(2)*ones(1,2)+sz(2)*[0 1];
    xpolys(x',y',5*ones(2,1));
    xstringb(orig(1)+sz(1)/16,orig(2)+sz(2)/16,arg1.graphics.exprs(1),3*sz(1)/8,7*sz(2)/8,'fill')
    xstringb(orig(1)+9*sz(1)/16,orig(2)+sz(2)/16,arg1.graphics.exprs(2),3*sz(1)/8,7*sz(2)/8,'fill')
    xf=40
    yf=60
    nout=1;

   if orient then
      out=[-1/14   0
           0       -1/7
	   1/14    0
	   -1/14   0]*diag([xf,yf])
      dy=sz(1)/(nout+1)
      xset('pattern',5)
      for k=1:nout
          xfpoly(out(:,1)+ones(4,1)*(orig(1)+sz(1)-dy*k),..
	      out(:,2)+ones(4,1)*(orig(2)),5)
       end
   else 
      out=[-1/14   0
           0       1/7
	   1/14    0
	   -1/14   0]*diag([xf,yf])
      dy=sz(1)/(nout+1)
      xset('pattern',5)
      for k=1:nout
          xfpoly(out(:,1)+ones(4,1)*(orig(1)+sz(1)-dy*k),..
	      out(:,2)+ones(4,1)*(orig(2)+sz(2)),5)
       end
    end

  ident = arg1.graphics.id
  if ident <> [] & ident <> '' then
    font=xget('font')
    xset('font', options.ID(1)(1), options.ID(1)(2))
    if ~orient then
       rectangle = xstringl(orig(1), orig(2), ident) ;
       w = max(rectangle(3), sz(1)) ;
       h = rectangle(4) * 1.3 ;
       xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;
    else
       rectangle = xstringl(orig(1), orig(2)+sz(2), ident) ;
       w = max(rectangle(3), sz(1)) ;
       h = rectangle(4) * 1.3 ;
       xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2)+ sz(2) , ident , w, h) ;
    end
    xset('font', font(1), font(2))
  end
  x=[];y=[]
case 'getinputs' then
  x=[];y=[];typ=[]
case 'getoutputs' then
  orig=arg1.graphics.orig;sz=arg1.graphics.sz;
  if arg1.graphics.flip then
    x=orig(1)+sz(1)/2
    y=orig(2)
  else
    x=orig(1)+sz(1)/2
    y=orig(2)+sz(2)
  end
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  model=arg1.model;
  exprs=graphics.exprs
  while %t do
    [ok,frequ,offset,exprs]=getvalue('Set block parameters',..
	['Sample time';'Offset'],..
          list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if frequ<0 then message("Frequency must be a positif number");ok=%f;end
    if abs(offset) > frequ then
       message("The |Offset| must be less than the Frequency");ok=%f
    end
    if ok then
    if or(model.rpar(:)<>[frequ;offset]) then needcompile=4;y=needcompile,end
      model.rpar=[frequ;offset]
      model.evtout=1
      model.firing=-1//compatibility
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
  end
  end
  needcompile=resume(needcompile)
case 'define' then
  model=scicos_model()
  model.sim='sampleclk'
  model.evtout=1
  model.rpar=[1,0]
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]

  exprs=[sci2exp(1);sci2exp(0)]
  x=standard_define([2 2],model,exprs,' ')
  x.graphics.id="S-CLK"
end
endfunction


