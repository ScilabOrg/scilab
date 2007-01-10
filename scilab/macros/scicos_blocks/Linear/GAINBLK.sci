function [x,y,typ]=GAINBLK(job,arg1,arg2)
// Copyright INRIA
  
  x=[];y=[];typ=[];
  
  select job
  
    //**------------------------------------------------------
    case 'plot' then //** ----------- PLOT -------------------
    pat=xget('pattern'); xset('pattern',default_color(0))
    orig=arg1.graphics.orig;
    sz=arg1.graphics.sz;
    orient=arg1.graphics.flip;
    gain=arg1.graphics.exprs;
    if orient then
      xx=orig(1)+[0 1 0 0]*sz(1);
      yy=orig(2)+[0 1/2 1 0]*sz(2);
      x1=0
    else
      xx=orig(1)+[0   1 1 0]*sz(1);
      yy=orig(2)+[1/2 0 1 1/2]*sz(2);
      x1=1/4
    end
    gr_i=arg1.graphics.gr_i;
    if type(gr_i)==15 then
      coli=gr_i(2);
      if coli<>[] then
	xfpolys(xx',yy',coli);
	pcoli=xget('pattern');xset('pattern',coli)
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
	xset('pattern',pcoli)
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
      else
	xpoly(xx,yy,'lines');
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
      end
    else
      xpoly(xx,yy,'lines');
      xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
    end
    xf=60
    yf=40
    nin=1;nout=1

    if orient then  //standard orientation
		    
      // set port shape
      out=[0  -1/14
	   1/7 0
	   0   1/14
	   0  -1/14]*diag([xf,yf])
      in= [-1/7  -1/14
	   0    0
	   -1/7   1/14
	   -1/7  -1/14]*diag([xf,yf])
      dy=sz(2)/(nout+1)
      xset('pattern',default_color(1))
      for k=1:nout
	xfpoly(out(:,1)+ones(4,1)*(orig(1)+sz(1)),..
	       out(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end

      dy=sz(2)/(nin+1)
      for k=1:nin
	xfpoly(in(:,1)+ones(4,1)*orig(1),..
	       in(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end
    else //tilded orientation
      out=[0  -1/14
	   -1/7 0
	   0   1/14
	   0  -1/14]*diag([xf,yf])
      in= [1/7  -1/14
	   0    0
	   1/7   1/14
	   1/7  -1/14]*diag([xf,yf])
      dy=sz(2)/(nout+1)
      xset('pattern',default_color(1))
      for k=1:nout
	xfpoly(out(:,1)+ones(4,1)*orig(1)-1,..
	       out(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end
      dy=sz(2)/(nin+1)
      for k=1:nin
	xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	       in(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end
    end
    xset('pattern',pat)
    
    //** ------- Identification ---------------------------
    ident = o.graphics.id
    gh_axes = gca(); //** get the Axes proprieties 
  
    // draw Identification
    if ident <> []& ident <> ''  then
  
      //** Save font state  
      axes_font_style = gh_axes.font_style ;
      axes_font_size  = gh_axes.font_size  ;
      //** axes_font_color = gh_axes.font_color ; //** optional
  
        gh_axes.font_style = options.ID(1)(1) ; 
        gh_axes.font_size  = options.ID(1)(2) ;
        //** font color not yet used 
        rectangle = xstringl(orig(1), orig(2), ident) ;
        w = max(rectangle(3), sz(1)) ;
        h = rectangle(4) * 1.3 ;
        xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;
  
     //** Restore font state 
     gh_axes.font_style = axes_font_style ;
     gh_axes.font_size  = axes_font_size  ;
     //** gh_axes.font_color = axes_font_color ; //** optional

  end
  //** ----- Identification End -----------------------------

    
    //**-----------------------------------------------------------
    case 'getinputs' then //** GET INPUTS 
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
      [ok,gain,exprs]=getvalue('Set gain block parameters',..
			       ['Gain'],list('mat',[-1,-1]),exprs(1))
      if ~ok then break,end
      if gain==[] then
	message('Gain must have at least one element')
      else
	[out,in]=size(gain)
	if out*in<>1 then
	  [model,graphics,ok]=set_io(model,graphics,...
				     list([in,-1],1),...
				     list([out,-1],1),[],[])
	else
	  [model,graphics,ok]=set_io(model,graphics,...
				     list([-1,-2],1),...
				     list([-1,-2],1),[],[])
	end
	if ok then
	  graphics.exprs=exprs
	  model.rpar=gain(:);
	  x.graphics=graphics;x.model=model
	  break
	end
      end
    end
    case 'define' then
    gain=1
    in=-1;out=-1
    in2=-2;out2=-2
    model=scicos_model()
    model.sim=list('gainblk',4)
    model.in=in
    model.out=out
    model.in2=in2
    model.out2=out2
    model.rpar=gain
    model.blocktype='c'
    model.dep_ut=[%t %f]
    
    exprs=[strcat(sci2exp(gain))]
    gr_i=''
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction
