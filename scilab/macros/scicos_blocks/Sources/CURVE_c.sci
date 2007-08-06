function [x,y,typ]=CURVE_c(job,arg1,arg2)
// Masoud NAJAFI 24/07/07 --------
// Copyright INRIA
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

  x=arg1
  model=arg1.model
  graphics=arg1.graphics
  rpar=model.rpar
  ipar=model.ipar
  N=ipar(1); 
  xx=rpar(1:N);yy=rpar(N+1:2*N)
  nipar=[0;0;0];
  nrpar=[]; 
  ok=%f;
  while %t do

    curwin=max(winsid())+1
    save_curwin=curwin;
    [nrpar,nipar,ok]=poke_point(xx,yy,ipar,rpar)
//    disp(nrpar);disp(nipar);
    curwin=save_curwin
    if ~ok then break,end
    nN=nipar(1);
    if (ok) then
      xp=find(nrpar(1:nN)>=0);
      if (xp<>[]) then 
	model.firing=nrpar(xp(1)); //first positive evenet
      else  
	model.firing=-1;
      end
      model.rpar=nrpar
      model.ipar=nipar
      x.model=model
      break
    end
  end
 case 'define' then  
  model=scicos_model()
  xx=[];yy=[]
  N=0;
  Order=1;
  PeriodicOption=0;
  model.sim=list('curve_c',4)
  model.in=[]
  model.out=1
  model.rpar=[xx;yy]
  model.ipar=[N;Order;PeriodicOption]
  model.blocktype='c'
  model.dep_ut=[%f %t]
  model.evtin=1
  model.evtout=1
  model.firing=0

  gr_i=['rpar=arg1.model.rpar;n=model.ipar(1);';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'rect=rpar(2*n+1:2*n+4);';
    'mxx=rect(3)-rect(1);';
    'mxy=rect(4)-rect(2);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
    gr_i=['txt=[''CURVE''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,[],gr_i)
end
endfunction


function [rpar,ipar,ok]=poke_point(ix,iy,iparin,rparin)
//
//! Masoud Najafi 08/2007
//origine: serge Steer, Habib Jreij INRIA 1993
// Copyright INRIA
[lhs,rhs]=argn(0)
//in line definition of get_click
deff('[btn,xc,yc,win,Cmenu]=get_click(flag)',[
'if ~or(winsid() == curwin) then   Cmenu = ''Quit'';return,end,';
'if argn(2) == 1 then';
'  [btn, xc, yc, win, str] = xclick(flag);';
'else';
'  [btn, xc, yc, win, str] = xclick();';
'end;'; 
'if btn == -100 then';
'  if win == curwin then';
'    Cmenu = ''Quit'';';
'  else';
'    Cmenu = ''Open/Set'';';
'  end,';
'  return,';
'end';
'if btn == -2 then';
'  xc = 0;yc = 0;';
'  try '    // added to handle unwanted menu actions in french version
'    execstr(''Cmenu='' + part(str, 9:length(str) - 1));';
'    execstr(''Cmenu='' + Cmenu);';
'  catch'
'    Cmenu=[]'    
'  end '    
'  return,';
'end';
'Cmenu=[]'])
 
ok=%f
if rhs==0 then ix=[];iy=[],end;
if rhs==1 then 
  xinfo(' No y provided');
  return
end


[x,y]=cleandata(ix,iy)
N=size(x,'*');

if rhs<=2 then
  NOrder=1;
  PeridicOption=0;
  ipar=[N;NOrder;PeridicOption]
  rpar=[]
else if rhs==3 then  
    NOrder=iparin(2);
    PeridicOption=iparin(3);
    ipar=iparin;
    rpar=[]
else if rhs==4 then  
    NOrder=iparin(2);
    PeridicOption=iparin(3);
    ipar=iparin;
    rpar=rparin    
end
end
end

Amp=1;wp=1; phase=0;offset=0;np1=20;
sAmp=1;sTp=1; sdelay=0;
sAmp2=1;sTp2=1;
Amp3=1;Tp3=2;Pw3=40;Pd3=0;
mean4=0;var4=1;seed4=0;sample4=1;np4=20;
min5=-1;max5=1;seed5=0;sample5=1;np5=20;

// bornes initiales du graphique
xmx=maxi(x);xmn=mini(x),xmn=max(xmn,0);
ymx=maxi(y);ymn=mini(y);
dx=xmx-xmn;dy=ymx-ymn
if dx==0 then dx=maxi(xmx/2,1),end
xmn=xmn;xmx=xmx+dx/10
if dy==0 then dy=maxi(ymx/2,1),end;
ymn=ymn-dy/10;ymx=ymx+dy/10;

rect=[xmn,ymn;xmx,ymx];
//===================================================================
// getf CURV_c.sci;getf poke_point.sci;scicos model.cos;
// Set menus and callbacks

//set("figure_style","new")
f=scf();
unsetmenu(curwin,'File',1) //clear
unsetmenu(curwin,'File',2) //select
unsetmenu(curwin,'File',6) //load
unsetmenu(curwin,'File',7) //close
//unsetmenu(curwin,'3D Rot.')
//unsetmenu(curwin,'Edit') //Edit
delmenu(curwin,'3D Rot.')
delmenu(curwin,'Edit')

menu_r=[];
menu_s=[];
menu_o=['zero order','linear','order 2','not_a_knot','periodic','monotone','fast','clamped']
menu_d=['Clear','Data Bounds','Change/Read from Scilab','Load from text f"+...
	"ile','Save to text file','Load from Excel','Periodic signal']
menu_t=['sine','sawtooth1','sawtooth2','pulse','random normal','random uniform']
menu_e=['Exit without save','Save/Exit']
MENU=['Autoscale','Spline','Data','Standards','Exit'];
menus=list(MENU,menu_s,menu_o,menu_d,menu_t,menu_e);

scam='menus(1)(1)'
w='menus(3)(';r=')';Orderm=w(ones(menu_o))+string(1:size(menu_o,'*'))+r(ones(menu_o))
w='menus(4)(';r=')';Datam=w(ones(menu_d))+string(1:size(menu_d,'*'))+r(ones(menu_d))
w='menus(5)(';r=')';Standm=w(ones(menu_t))+string(1:size(menu_t,'*'))+r(ones(menu_t))
w='menus(6)(';r=')';Exitm=w(ones(menu_e))+string(1:size(menu_e,'*'))+r(ones(menu_e))

execstr('Autoscale_'+string(curwin)+'=scam')
execstr('Spline_'+string(curwin)+'=Orderm')
execstr('Data_'+string(curwin)+'=Datam')
execstr('Standards_'+string(curwin)+'=Standm')
execstr('Exit_'+string(curwin)+'=Exitm')

addmenu(curwin,MENU(1))
addmenu(curwin,MENU(2),menu_o)
addmenu(curwin,MENU(3),menu_d)
addmenu(curwin,MENU(4),menu_t)
addmenu(curwin,MENU(5),menu_e)
//===================================================================
//initial draw
f.pixmap='off';
drawlater();
a=gca(f);
a.data_bounds=rect;
a.axes_visible='on';
a.clip_state='on';
xtitle( '', 'time', 'Output' ) ; 
a.title.font_size=2;
a.title.font_style=4;
a.title.foreground=2;

a.grid=[2 2];
xpolys(x,y,[-1]);   //children(2)
xpolys(x,y,[5]);    //children(1)
splines=a.children(1).children
points=a.children(2).children
//---------------------------------------
[rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
drawnow();
// -- boucle principale
while %t then //=================================================
  N=prod(size(x))
  [btn,xc,yc,win,Cmenu]=get_click()    
  if ((win>0) & (win<>curwin)) then
    Cmenu='Mouse click is Offside!';
  end
  if Cmenu==[] then Cmenu='edit',end
  if (Cmenu=='Exit') |(Cmenu=='Quit' ) then, ipar=[];rpar=[];ok=%f;return; end
  //-------------------------------------------------------------------
  if ((Cmenu=='zero order') | (Cmenu=='linear') | (Cmenu=='order 2')| ...
      (Cmenu=='not_a_knot')| (Cmenu=='periodic')| (Cmenu=='monotone')| ...
      (Cmenu=='fast')| (Cmenu=='clamped')) then
        
    select  Cmenu
     case 'zero order' then
      NOrder=0;
     case 'linear' then
      NOrder=1;
     case 'order 2' then
      NOrder=2;
     case 'not_a_knot' then
      NOrder=3;
     case 'periodic' then
      NOrder=4;
     case 'monotone' then
      NOrder=5;
     case 'fast' then
      NOrder=6;
     case 'clamped' then
      NOrder=7;
    end
    ipar(2)=NOrder;
   [rpar,ipar]=AutoScale(a,x,y,ipar,rpar)  
  end
  //-------------------------------------------------------------------  
  select Cmenu
   case 'Data Bounds' then
      rectx=findrect(a);
      [mok,xmn1,xmx1,ymn1,ymx1]=getvalue('Enter new bounds',['xmin';'xmax'; ...
		    'ymin';'ymax'],list('vec',1,'vec',1,'vec',1,'vec',1), ...
				     string(rectx))
      //drawlater();
      if mok then 
	if (xmn1>xmx1|ymn1>ymx1) then
	  xinfo('Incorrect bounds')
	  mok=%f;
	end
	if xmn1<0 then
	  xinfo('X soululd be positive')
	  mok=%f;
	end
	if mok then 
	  a.data_bounds=[xmn1,ymn1;xmx1,ymx1];
	end
      end
      //drawnow();//show_pixmap(); 
    //-------------------------------------------------------------------  
   case 'Autoscale' then 
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
      //-------------------------------------------------------------------  
   case 'Periodic signal' then 
    if PeridicOption==1 then, ans0='yes',else, ans0='no',end;
    [mok,myans]=getvalue('Generating peridic signal',['yes/no'],list('str',1),list(ans0));
    if ((myans=='yes')|(myans=='YES')|(myans=='Yes')) then,PeridicOption=1,else,PeridicOption=0;end;
    ipar(3)=PeridicOption;
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
    //-------------------------------------------------------------------
   case 'sine' then 
    [mok,Amp,wp,phase,offset,np1]=getvalue(' Sine parameters', ...
				['Amplitude';'Frequency(rad/sec)'; ...
		    'Phase(rad)';'Bias';'number of points'],list('vec',1,'vec',1,'vec',1, ...
					   'vec',1,'vec',1),list(string(Amp), ...
						  string(wp), ...
						  string(phase),string(offset),string(np1)))
    if mok & wp>0 & np1>3 then
      NOrder=3;
      ipar(2)=NOrder;
      phase=atan(tan(phase));
      x=linspace(0,%pi*2/wp,np1)';
      y=Amp*sin(wp*x+phase)+offset;
//      rpar=[Amp;wp;phase;offset]	    
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar)  
    end
    //-------------------------------------------------------------------
   case 'sawtooth1' then 
    [mok,sAmp,sTp,sdelay]=getvalue('Sawtooth signal parameters', ...
			  ['Amplitude';'Period';'delay'], ...
			  list('vec',1,'vec',1,'vec',1),list(string(sAmp),string(sTp),string(sdelay)))   
    if mok & sTp>0 then
      NOrder=1;
      ipar(2)=NOrder;
      x=[0;sdelay;sTp];
      y=[0;0;sAmp];
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar);
    end
    //-------------------------------------------------------------------
   case 'sawtooth2' then     
    [mok,sAmp2,sTp2]=getvalue('Sawtooth signal parameters', ...
			  ['Amplitude';'Period'], ...
			  list('vec',1,'vec',1),list(string(sAmp2),string(sTp2)))    
    if mok & sTp2>0 then
      NOrder=1;
      ipar(2)=NOrder;
      x=[0;sTp2];
      y=[sAmp2;-sAmp2];
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar);
    end
    //-------------------------------------------------------------------
   case 'pulse' then
    [mok,Amp3,Tp3,Pw3,Pd3]=getvalue('Square wave pulse signal', ...
				    ['Amplitude';'Period (sec)';'Pulse width(% o"+...
		    "f period)';'Phase delay (sec)'],list('vec',1, ...
						  'vec',1,'vec',1,'vec', ...
						  1),list(string(Amp3), ...
						  string(Tp3),string(Pw3),string(Pd3)))        
    if mok & Tp3>0  then
      NOrder=0;
      ipar(2)=NOrder;
      if (Pd3>0) then x=0;y=0;else x=[];y=[]; end
      //otherwise there	would be double	points at 0
      if Pw3>0 then 
	x=[x;Pd3; Pw3*Tp3/100+Pd3;Tp3];
	y=[y;Amp3;0;0];	
      else
	x=0;y=0;
      end      
      
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar);
    end
     //-------------------------------------------------------------------
   case 'random normal' then
    [mok,mean4,var4,seed4,sample4,np4]=getvalue('Normal (Gaussian) random signal', ...
				    ['Mean';'Variance';'Initial seed';'Sample time';'Number of points'],list('vec',1, ...
						  'vec',1,'vec',1,'vec', ...
						  1,'vec',1),list(string(mean4), ...
						  string(var4), ...
						  string(seed4),string(sample4),string(np4)))        
    if mok & sample4>0 then
      NOrder=0;
      ipar(2)=NOrder;      
      rand('normal');  rand('seed',seed4);
      x=0:sample4:sample4*(np4-1);x=x(:);
      y=mean4+sqrt(var4)*rand(np4,1);
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar);
    end
     //-------------------------------------------------------------------
   case 'random uniform' then
    [mok,min5,max5,seed5,sample5,np5]=getvalue('Uniform random signal', ...
				    ['Minimum';'Maximum';'Initial seed';'Sample time';'Number of points'],list('vec',1, ...
						  'vec',1,'vec',1,'vec', ...
						  1,'vec',1),list(string(min5), ...
						  string(max5), ...
						  string(seed5),string(sample5),string(np5)))        
    if mok & sample5>0 then
      NOrder=0;
      ipar(2)=NOrder;      
       rand('uniform'); rand('seed',seed5);
      x=0:sample5:sample5*(np5-1);x=x(:);
      y=min5+(max5-min5)*rand(np5,1);
      [rpar,ipar]=AutoScale(a,x,y,ipar,rpar);
    end   
    //-------------------------------------------------------------------
   case 'Save/Exit' then
    NOrder=ipar(2);
    PeridicOption=ipar(3);

    if (NOrder==4) then // periodic spline
      y(N)=y(1);
    end
    
    if (NOrder>=2) then 
      rpar=[x;y;rpar];
    else
      if (NOrder==0|NOrder==1)
	rpar=[x;y]
      end
    end
    ok=%t
    delete(f);
    return 
    //-------------------------------------------------------------------
   case 'Exit without save' then 
    ipar=[];
    rpar=[];
    ok=%f
    delete(f);
    return
    //-------------------------------------------------------------------
   case 'Clear' then    
    x=[0];
    y=[0];
    NOrder=0;
    ipar(2)=NOrder;
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
    //----------------------------------------------------------------
   case 'Change/Read from Scilab' then
    [mok,xx,yy]=getvalue('Enter x and y data',['x';'y'],list('vec',-1,'vec',-1),list(strcat(sci2exp(x)),strcat(sci2exp(y))));
    if mok then  
      [x,y]=cleandata(xx,yy)
    end
    NOrder=1
    ipar(2)=NOrder;
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
    //---------------------------------------------------------------  
   case 'Load from Excel' then
    [tok,xx,yy]=ReadExcel()
    if tok then
      x=xx;  y=yy;
    end
    NOrder=1
    ipar(2)=NOrder;
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
   //---------------------------------------------------------------       
    case 'Load from text file' then
    [tok,xx,yy]=ReadFromFile()
    if tok then
      x=xx;  y=yy;
    end
    NOrder=1
    ipar(2)=NOrder;
    [rpar,ipar]=AutoScale(a,x,y,ipar,rpar) 
   //---------------------------------------------------------------     
   case 'Save to text file' then    
    [sok]=SaveToFile(x,y)
    //---------------------------------------------------------------       
   case 'Replot' then
    if x<>[]&y<>[] then 
      drawlater();
      points.data=[x,y];
      [rpar,ipar]=drawSplin(a,x,y,ipar,rpar);
      //show_pixmap(); 
      drawnow()
    end
    //----------------------------------------------------------
   case 'edit' then 
    HIT=%f
    if N<>0 then
      dist=((x-ones(N,1)*xc)/dx)^2+((y-ones(N,1)*yc)/dy)^2
      [m,k]=mini(dist);m=sqrt(m)
      rectx=a.data_bounds;
      ex=abs(rectx(2,1)-rectx(1,1))/80;
      ey=abs(rectx(2,2)-rectx(1,2))/80;
      if (abs(xc-x(k))<ex & abs(yc-y(k))<ey) then 
	HIT=%t
      end
    end
    
    //_________________________
  //  if ~((NOrder==-1|NOrder==-2|NOrder==-3|NOrder==-4)) then
      if (~HIT)&(btn==0 | btn==3) then    // add point
	if (xc>=0) then 
	  if (xc==0) then 
	    zz=find(x==0);
	    x(zz)=[];y(zz)=[];
	  end 
	  x=[x;xc];
	  y=[y;yc];
	  [x,k2]=gsort(x,'r','i');y=y(k2)
	  f.pixmap='on';
	  drawlater();
	  points.data=[x,y];
	  [rpar,ipar]=drawSplin(a,x,y,ipar,rpar);  
	  show_pixmap(); 
	  drawnow()
	  f.pixmap='off';
	end	      
      end
      
      if (HIT)&(btn==2 | btn==5) then  //   remove point
	f.pixmap='on';
	if (x(k)>0) |( x(k)==0 & (size(find(x==0),'*')>1)) then 
	  x(k)=[];  y(k)=[];
	end
	drawlater();
	points.data=[x,y];
	[rpar,ipar]=drawSplin(a,x,y,ipar,rpar);  
	show_pixmap(); 
	drawnow()
      f.pixmap='off';
      end   

      if (HIT)&(btn==0) then             // move point
	f.pixmap='on';
	[x,y,rpar,ipar]=movept(a,x,y,ipar,rpar,k)   
	f.pixmap='off';
      end
  //  end
    //_________________________________
   
  end
  //----------------------------------------------------------
end
endfunction
//========================================================================
function [orpar,oipar]=drawSplin(a,x,y,iipar,irpar)
  points=a.children(2).children
  splines=a.children(1).children
  order=iipar(2);
  periodicoption=iipar(3);
  orpar=irpar;
  N=size(x,'*');// new size of x
  X=[];Y=[];
  NP=10;
  for i=1:N-1
    X=[X;linspace(x(i),x(i+1),NP)']; // pour tous sauf "linear" et "zero order"
  end
  
  METHOD=getmethod(order);
  if periodicoption==1 then PERIODIC='periodic, T='+string(x(N)-x(1));
  else PERIODIC='aperiodic';end  
  a.title.text=[string(N)+' points,  '+'Method: '+METHOD+',  '+PERIODIC];

  if (N==0) then, return; end
  if (N==1) then, order=0; end

  if (METHOD=='zero order') then 
    X=x(1);Y=y(1);
    for i=1:N-1
      X=[X;x(i);x(i+1);x(i+1)];
      Y=[Y;y(i);y(i);y(i+1)];
    end  
  end    
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='linear') then
    X=[];
    for i=1:N
      X=[X;x(i)];
      Y=[Y;y(i)];
    end  
  end
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (N>2) & (METHOD=='order 2') then
    
    derN=0;Der_at_XN=0;
    [costs,Der_at_XN]=optim(costx,derN)        
    [cost,Z]=cost_sp2(Der_at_XN);
    A=Z(1:N-1);
    B=Z(N:2*N-2);
    C=Z(2*N-1:3*N-3);
    
    for j=1:size(X,'*')
      for i=N-1:-1:1
	if X(j)>=x(i) then,break;end
      end
      Y(j)=A(i)*(X(j)-x(i))^2+B(i)*(X(j)-x(i))+C(i);
    end    
    orpar=matrix(Z,-1,1)   
  end  
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='not_a_knot') then
    try
    d = splin(x, y, METHOD);
    Y = interp(X, x, y, d);    
    orpar=d(:);
    catch
     xinfo('ERROR in SPLINE: '+METHOD)
    end
    
  end
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='periodic') then
    if y(1)<>y(N) then 
      y(N)=y(1)
    end
    try 
      d = splin(x, y,METHOD);
      Y = interp(X, x, y, d);  
      orpar=d(:);
    catch
    xinfo('ERROR in SPLINE: '+METHOD)
    end
  end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='monotone' ) then
    try
      d = splin(x, y, METHOD);
      Y = interp(X, x, y, d);  
      orpar=d(:);
    catch
    xinfo('ERROR in SPLINE: '+METHOD)
    end
  
  end
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='fast') then
    try
      d = splin(x, y, METHOD);
      Y = interp(X, x, y, d);    
      orpar=d(:);
    catch
      xinfo('ERROR in SPLINE:  '+METHOD)    
    end  
  end  
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (METHOD=='clamped') then
    try
      d = splin(x, y, METHOD,[0;0]);
      Y = interp(X, x, y, d);    
      orpar=d(:);
    catch
      xinfo('ERROR in SPLINE: '+METHOD)    
    end
  end  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if  (METHOD=='Sine') then // not in use
    Amp=irpar(2*N+1);
    wp=irpar(2*N+2);
    phase=irpar(2*N+3);
    offset=irpar(2*N+4);
    if wp>0 then 
      X=linspace(0,2*%pi/wp,NP)';
      Y = Amp*sin(wp*X+phase)+offset;
    else
      X=irpar(1:N);
      Y=irpar(N+1:2*N);
    end
  end  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (periodicoption==1) then 
    X=[X;X($)];
    Y=[Y;Y(1)];
  else
    xmx=maxi(points.data(:,1));  xmn=mini(points.data(:,1));
    XMX=xmx; XMN=max(0,xmn);
    dx=XMX-XMN; if dx==0 then dx=maxi(XMX/2,1),end
    XMX=XMX+dx/10;
    xmx1=max(a.x_ticks.locations)
    XMX=max(XMX,xmx1);
    X=[X;XMX];
    Y=[Y;Y($)];
  end
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   splines.data=[X,Y];    
   oipar=[N;iipar(2);periodicoption]
endfunction
//=============================================================
function [xt,yt,orpar,oipar]=movept(a,x,y,iipar,irpar,k)
//on bouge un point existant
  points=a.children(2).children
  splines=a.children(1).children  
  oipar=iipar
  orpar=irpar
  order=iipar(2);

  if (x(k)==0) then 
    zz=find(x==0);
    x(zz)=[];y(zz)=[];
    ZERO_POINT=%t
  else
    x(k)=[];
    y(k)=[]; 
    ZERO_POINT=%f
  end 

  btn=-1

  while ~(btn==3 | btn==0| btn==10| btn==-5)
    rep=xgetmouse([%t %t]); xc=rep(1);yc=rep(2);btn=rep(3);
    if (ZERO_POINT) then 
      xc=0;
    else
      if (xc<=0) then 
	zz=find(x==0);
	x(zz)=[];y(zz)=[];
	ZERO_POINT=%t;
	xc=0;
      end
    end
  
    xt=[x;xc];
    yt=[y;yc];
    [xt,k2]=gsort(xt,'r','i');yt=yt(k2)
        
    drawlater();
    points.data=[xt,yt];
    [orpar,oipar]=drawSplin(a,xt,yt,oipar,orpar); 
    show_pixmap();  
    drawnow()
  end

endfunction

//==========================================================
function   rectx=findrect(a) 
  splines=a.children(1).children  
  points=a.children(2).children

  if (points.data==[]) then 
    rectx=a.data_bounds;
    return;
  end    


  ymx1=max(splines.data(:,2));  ymn1=min(splines.data(:,2))
  
  xmx=maxi(points.data(:,1));xmn=mini(points.data(:,1));
  ymx=maxi(points.data(:,2));ymn=mini(points.data(:,2));

  
  XMX=xmx;            XMN=max(0,xmn);
  YMX=max(ymx,ymx1);  YMN=min(ymn,ymn1);
  
  dx=XMX-XMN;dy=YMX-YMN
  if dx==0 then dx=maxi(XMX/2,1),end
  XMN=XMN;XMX=XMX+dx/10
  if dy==0 then dy=maxi(YMX/2,1),end;
  YMN=YMN-dy/10;YMX=YMX+dy/10;  
  rectx=[XMN,YMN;XMX,YMX];
endfunction

//============================================================
function [tok,xe,ye]=ReadExcel()
  TA=['A';'B';'C';'D';'E';'F';'G';'H';'I';'J';'K';'L';'M';'N';'O';'P'; ...
      'Q';'R';'S';'T';'U';'V';'W';'X';'Y';'Z';'a';'b';'c';'d';'e';'f'; ...
      'g';'h';'i';'j';'k';'l';'m';'n';'o';'p';'q';'r';'s';'t';'u';'v'; ...
      'w';'x';'y';'z'];
  TN=['0','1','2','3','4','5','6','7','8','9'];
  xe=[];ye=[];tok=%f;
  while %t
    [zok,filen,sheetN,xa,ya]=getvalue('Excel data file ',['Filename';'Sheet #"+...
		    " ';'X[start:Stop]';'Y[start:stop]'],list('str',1, ...
						  'vec',1,'str',1, ...
						  'str',1), ...
				      list(['Classeur1.xls'],['1'],['C5:C25'],['D5:D25']));   
    if ~zok then break,end

    try
        [fd,SST,Sheetnames,Sheetpos] = xls_open(filen);
    catch
        xinfo('Scicos canot find the excel file:'+filen);
	break;
    end 
    try  
    N=size(Sheetnames,'*');
    if ((sheetN<=N) &(sheetN>0)) then 
      [Value,TextInd] = xls_read(fd,Sheetpos(sheetN))
      mclose(fd)
    end
    xa=strsubst(xa,' ',''); px=strindex(xa,':'); 
    ya=strsubst(ya,' ',''); py=strindex(ya,':');
    x1=part(xa,1:px-1); x2=part(xa,px+1:length(xa));
    y1=part(ya,1:py-1); y2=part(ya,py+1:length(ya));

    x1p=min(strindex(x1,TN));
    if x1p==[] then, xinfo('Bad address in X:'+x1); break, end
    x11=part(x1,1:x1p-1);x12=part(x1,x1p:length(x1));
    
    x2p=min(strindex(x2,TN));
    if x2p==[] then, xinfo('Bad address in X:'+x2); break, end
    x21=part(x2,1:x2p-1);x22=part(x2,x2p:length(x2));
    
    y1p=min(strindex(y1,TN));
    if y1p==[] then, xinfo('Bad address in Y:'+y1); break, end
    y11=part(y1,1:y1p-1);y12=part(y1,y1p:length(y1));

    y2p=min(strindex(y2,TN));
    if y2p==[] then, xinfo('Bad address in Y:'+y2); break, end
    y21=part(y2,1:y2p-1);y22=part(y2,y2p:length(y2));
       
    // x11 x12: x21 x22
    
    lx11=length(x11);lx21=length(x21);
    ly11=length(y11);ly21=length(y21)
    xstC=0;for i=1:lx11,xstC=xstC+modulo(find(TA==part(x11,lx11-i+1)),26)*26^(i-1);end
    xenC=0;for i=1:lx21,xenC=xenC+modulo(find(TA==part(x21,lx21-i+1)),26)*26^(i-1);end
    ystC=0;for i=1:ly11,ystC=ystC+modulo(find(TA==part(y11,ly11-i+1)),26)*26^(i-1);end
    yenC=0;for i=1:ly11,yenC=yenC+modulo(find(TA==part(y21,ly21-i+1)),26)*26^(i-1);end
        
    xstR=evstr(x12);
    xenR=evstr(x22);
    ystR=evstr(y12);
    yenR=evstr(y22);
    
    [mv,nv]=size(Value)

    if ~(xstR<=mv & xstR>0 & xenR<=mv & xenR>0&ystR<=mv & ystR>0&yenR<=mv&yenR>0 ) then 
      xinfo('error in Row data addresses'); break
    end
    if ~(xstC<=nv & xstC>0 & xenC<=nv & xenC>0&ystC<=nv & ystC>0&yenC<=nv&yenC>0 ) then 
      xinfo('error in Column data addresses'); break
    end
     
    xe=Value(min(xstR,xenR):max(xstR,xenR),min(xstC,xenC):max(xstC,xenC));
    ye=Value(min(ystR,yenR):max(ystR,yenR),min(ystC,yenC):max(ystC,yenC));

    [xe,ye]=cleandata(xe,ye)
    tok=%t;break,
    catch
         xinfo(' Scicos cannot read your Excel file, please verify"+...
		   " the parameters '); 	 
	 break
    end	 
  end
  
endfunction
//---------------------------------------------------------------
function [xo,yo]=cleandata(xe,ye)
  xe=xe(:)
  ye=ye(:)
  
  [nx,mx]=size(xe);// adjusting the x and y sizez
  [ny,my]=size(ye);
  N=min(nx,ny);
  xe=xe(1:N,:);
  ye=ye(1:N,:);

  // checking for NULL
  for i=1:N
    if (xe(i)<>xe(i)) then 
      xinfo('x contains no data:x('+string(i)+')'); 
      return;
    end
    if (ye(i)<>ye(i)) then 
      xinfo('Y contains no data:y('+string(i)+')'); 
      return;
    end      
  end
  zz=find(xe<0);xe(zz)=[];ye(zz)=[]
  if (find(xe==0)==[]) then // add zero point
    xe(N+1)=0;
    ye(N+1)=0;
  end
  
  [xo,k2]=gsort(xe,'r','i');
  yo=ye(k2)    
 
endfunction
//---------------------------------------------------------------
function  [orpar,oipar]=AutoScale(a,x,y,inipar,inrpar)   
  
  drawlater();    
  oipar=inipar
  orpar=inrpar
  points=a.children(2).children
  splines=a.children(1).children
  points.data=[x,y];
  splines.data=[x,y];
  [orpar,oipar]=drawSplin(a,x,y,oipar,orpar);
  rectx=findrect(a);     
  a.data_bounds=rectx;
  show_pixmap(); 
  drawnow()
endfunction
//============================
function METHOD=getmethod(order)
  select order
   case 0 then, METHOD='zero order'
   case 1 then, METHOD='linear'
   case 2 then, METHOD='order 2'
   case 3 then, METHOD='not_a_knot'
   case 4 then, METHOD='periodic'
   case 5 then, METHOD='monotone'
   case 6 then, METHOD='fast'
   case 7 then, METHOD='clamped'
  end
endfunction
//===========================
function [cost,Z]=cost_sp2(derN,x,y)
N=size(x,'*')-1;
  
A=zeros(N*3,N*3);
B=zeros(3*N,1);
for i=1:N
   j=3*(i-1)+1;
   A(j,i+2*N)=1;
   B(j)=y(i);
   A(j+1,i)=(x(i+1)-x(i))^2;
   A(j+1,i+N)=x(i+1)-x(i);
   A(j+1,i+2*N)=1;
   B(j+1)=y(i+1);
end

for i=1:N-1
   j=3*(i-1)+1;
   A(j+2,i)=2*(x(i+1)-x(i));
   A(j+2,i+N)=1;   
   A(j+2,i+N+1)=-1;
end

 A(3*N,N)=(x(N+1)-x(N))*2;
 A(3*N,2*N)=1;     
 B(3*N)=derN;

Z=A\B;

a=Z(1:N);
b=Z(N+1:2*N);
c=Z(2*N+1:3*N);

for i=1:N
ders(i)=(b(i))^2
end
ders(N+1)=(2*a(N)*(x(N+1)-x(N))+b(N))^2;

//cost=max(ders);
cost=sum(ders);

endfunction
//====================================== 
function  dmaxxx=Grad_cost_sp2(derN,x,y)
N=size(x,'*')-1;
eps=1e-5
[mx1,z]=cost_sp2(derN);
dd=derN+eps;
[mx2,z]=cost_sp2(dd);
dmaxxx=(mx2-mx1)/eps;
endfunction
//=======================================
function [f,g,ind]=costx(derN,ind)
  [f,z]=cost_sp2(derN);
  g=Grad_cost_sp2(derN);
endfunction
//=======================================
function [sok,xe,ye]=ReadFromFile()
  xe=[];ye=[];sok=%f;
  while %t
    [sok,filen,Cformat,Cx,Cy]=getvalue('Text data file ',['Filename';'Reading [C] f"+...
		    "ormat';'Abscissa column';'Output"+...
		    " column'],list('str',1,'str',1,'vec',1,'vec',1), ...
				      list(['mydatafile.dat'],['%g %g'],['1'],['2']));       
    if ~sok then break,end
    px=strindex(Cformat,'%');
    NC=size(px,'*');    
    if NC==[] then, xinfo('Bad format in reading data file');sok=%f;break;end
    Lx=[];
    try
      fd=mopen(filen,'r');
      Lx=mfscanf(-1,fd,Cformat);
      mclose(fd);
    catch
      xinfo('Scicos canot open the data file:'+filen);
      break;
    end 

    [nD,mD]=size(Lx);
    if ((mD==0) | (nD==0)) then,  xinfo('No data read');sok=%f;break;end
    if (mD<>NC) then, xinfo('Bad format');sok=%f;break;end
    
    xe=Lx(:,Cx);ye=Lx(:,Cy);
    [xe,ye]=cleandata(xe,ye)
    sok=%t;break,
 end 
endfunction
//=======================================
function [sok]=SaveToFile(xe,ye)
  sok=%f;
  while %t
    [sok,filen,Cformat]=getvalue('Text data file ',['Filename';'Writing [C] f"+...
		    "ormat'],list('str',1,'str',1), ...
				      list(['mydatafile.dat'],['%g %g']));       
    if ~sok then break,end
    px=strindex(Cformat,'%');
    NC=size(px,'*');    
    if NC<>2 then, xinfo('Bad format in writing data file');sok=%f;break;end

    Cformat=Cformat+'\n';
    
    try
      fd=mopen(filen,'w');
      mfprintf(fd,Cformat,xe,ye);
      mclose(fd);
    catch
      xinfo('Scicos canot open the data file:'+filen);
      break;
    end 

    sok=%t;break,
 end 
endfunction
//=========================================================
function unjouronsensertjesuissur()
      if (METHOD=='Sine') then
      N=size(x,'*');
      if (N>1) then 
	if (k==1) then
	  Amp=y(1);
	  yc=min(yc,Amp);
	  phase=asin(yc/Amp)
	  wp=2*%pi/x(2);
	  x=[%pi/2/wp-phase;%pi*2/wp-phase];
	  y=Amp*sin(wp*x+phase);	  
	end
	if (k==2) then
	  xc=x(2)/4; // x(3) is shifted to x(2) when moving 
	end
	if (k==3) then
	  yc=0;
	  x(2)=xc/4;
	end
      else
	xc=0;
	yc=0;
      end
      
    end 
  endfunction
  
