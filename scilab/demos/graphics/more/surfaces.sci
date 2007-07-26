
function hole3d()
expr=['// Holes in surfaces using %inf'
    '// Copyright INRIA'
    't=linspace(-%pi,%pi,40);z=sin(t)''*cos(t);'
    'z1=find(abs(z) > 0.5);'
    'z(z1)=%inf*z1;'
    'plot3d1(t,t,z);'
    'a=gca(); a.title.text=""surface with holes 1"";'
    'a.title.font_size=3;'];
     x_message(expr);
 	   execstr(expr);
endfunction

function hole3d1()
expr=['// Holes in surfaces using %inf '
     'deff(''[x,y,z]=sph(alp,tet)'',[''x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)'';'
     '''y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)'';'
     '''z=r*sin(alp)+orig(3)*ones(tet)'']);'
     'r=1;orig=[0 0 0];'
     'x=linspace(-%pi/2,%pi/2,40);y=linspace(0,%pi*2,20);' 
     'x(5:8)=%inf*ones(5:8);'
     'x(30:35)=%inf*ones(30:35);' 
     '[x1,y1,z1]=eval3dp(sph,x,y);'
     'plot3d1(x1,y1,z1)'
     'a=gca(); a.title.text=""surface with holes 2"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction

function sphere()
expr=['u = linspace(-%pi/2,%pi/2,40);'
     'v = linspace(0,2*%pi,20);'
     'x= cos(u)''*cos(v);'
     'y= cos(u)''*sin(v);'
     'z= sin(u)''*ones(v);'
     'plot3d2(x,y,z);'
     'a=gca(); a.title.text=""sphere"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function shell()
expr=['u = linspace(0,2*%pi,40);'
     'v = linspace(0,2*%pi,20);'
     'x= (cos(u).*u)''*(1+cos(v)/2);'
     'y= (u/2)''*sin(v);'
     'z= (sin(u).*u)''*(1+cos(v)/2);'
     'plot3d2(x,y,z);'
     'a=gca(); a.title.text=""shell"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function spiral()
expr=['[r,a]=field(0:0.1:1,0:%pi/8:6*%pi);'
     'z=a/8;'
     'x=r.*cos(a).*(1-a/20);'
     'y=r.*sin(a).*(1-a/20);'
     'z=z-1.5;'
     'plot3d2(x,y,z);'
     'a=gca(); a.title.text=""spiral"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function rings()
expr=['rr=0.2;'
     't=linspace(0,2*%pi,10);'
     's=linspace(0,2*%pi,41); n=length(s);'
     'r=dup(1+cos(t)*rr,n)''; m=length(t);'
     'x=dup(cos(s),m).*r; y=dup(sin(s),m).*r;'
     'z=dup(sin(t)*rr,n)'';'
     'X=[x;(x+1.3);(x-1.3)];'
     'Y=[y;-z;-z];'
     'Z=[z;y;y];'
     'plot3d2(X,Y,Z,[m,2*m]);'
     'a=gca(); a.title.text=""rings"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function torus()
expr=['// some torus type bodies.'
     'x=linspace(0,2*%pi,40);'
     'y=linspace(0,2*%pi,20)'';'
'// a torus with a thick and a thin side.'
     'factor=1.5+cos(y)*(cos(x)/2+0.6);'
     'X=factor*diag(cos(x));'
     'Y=factor*diag(sin(x));'
     'Z=sin(y)*(cos(x)/2+0.6);'
     'plot3d2(X,Y,Z);'
     'a=gca(); a.title.text=""torus"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function torus1()     
expr=['// a deformed torus'
     'x=linspace(0,2*%pi,40);'
     'y=linspace(0,2*%pi,20)'';'
     'factor=1.5+cos(y);'
     'X=factor*cos(x);'
     'Y=factor*sin(x);'
     'Z=sin(y)*ones(x)+ ones(y)*cos(2*x);'
     'plot3d2(X,Y,Z);'
     'a=gca(); a.title.text=""torus 1"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction
     
function moebius()
expr=['// the Moebius band'
     't=linspace(-1,1,20)'';'
     'x=linspace(0,%pi,40);'
     'factor=2+ t*cos(x);'
     'X=factor*diag(cos(2*x));'
     'Y=factor*diag(sin(2*x));'
     'Z=t*sin(x);'
     'plot3d2(X,Y,Z);'
     'a=gca(); a.title.text=""moebius"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction

function tube(nn)
expr=['// some tube like bodies.'
     'x=linspace(0,2*%pi,nn);'
     '//  atomic modell or so.'
     'y=0.1+[sin(linspace(0,%pi,15)),1.5*sin(linspace(0,%pi,10)),sin(linspace(0,%pi,15))];'
     'cosphi=dup(cos(x),length(y));'
     'sinphi=dup(sin(x),length(y));'
     'f=dup(y'',length(x));'
     'x1=f.*cosphi;     y1=f.*sinphi;'
     'z=dup(linspace(-2,2,prod(size(y)))'',prod(size(x)));'
     'plot3d2(x1,y1,z,-1,35,70);'
     'a=gca(); a.title.text=""tube"";'
     'a.title.font_size=3;'];
      x_message(expr);
 	    execstr(expr);
endfunction

function bh(nn)
expr=['// a black hole'
     'x=linspace(0,2*%pi,nn);'
     't=linspace(0,1,20);'
     'cosphi=dup(cos(x),length(t));'
     'sinphi=dup(sin(x),length(t));'
     'f=dup((t.*t+0.2)'',length(x));'
     'plot3d2(f.*cosphi,f.*sinphi,dup(t''.*2-1,length(x)));'
     'a=gca(); a.title.text=""black hole"";'
     'a.title.font_size=3;'];
       x_message(expr);
 	    execstr(expr);
endfunction

