function demo_riemann()

  //demo_help demo_riemann
  clf();
  curFig = gcf();
  xselect();
  curFig.figure_style = "new";
  toolbar(curFig.figure_id,"off");
  kp = curFig.pixmap;
  curFig.pixmap = 'on';
  
  C=hotcolormap(200);C=C(1:$-40,:); 
  curFig.figure_size = [700,900];

  curFig.color_map = C;
  curAxe = gca();
  n=size(C,1)
  curFig.background = n+1;
  curAxe.background = n+1; 
  curAxe.foreground = n+2;
  
  curAxe.font_color = n+2;
  curAxe.x_label.foreground = n+2;
  curAxe.y_label.foreground = n+2;
  curAxe.z_label.foreground = n+2;
  
  [z,s]=cplxroot(3,20) //compute
  curFig.pixmap = 'on';
  cplxmap(z,s,163,69)  //draw

  realtimeinit(0.1)
  for i=1:10,
    k = i ;
    realtime(k),
    drawlater();
    curAxe.rotation_angles = [69+k/2,163+k];
    drawnow();
    show_pixmap();
  end

  curFig.pixmap = kp;
  
  realtimeinit(0.1),for k=1:60,realtime(k),end
  delete(gcf());
endfunction




function cplxmap(z,w,varargin)
//cplxmap(z,w,T,A,leg,flags,ebox)
//cplxmap Plot a function of a complex variable.
//       cplxmap(z,f(z))
// Copyright INRIA
x = real(z);
y = imag(z);
u = real(w);v = imag(w);
//ncols=size(xget('colormap'),1)
ncols=size(curFig.color_map,1)
[X,Y,U]=nf3d(x,y,u);
[X,Y,V]=nf3d(x,y,v);
Colors = sum(V,'r');
Colors = Colors - min(Colors);
Colors = int((ncols-1)*Colors/max(Colors)+1);
plot3d(X,Y,list(U,Colors),varargin(:))
endfunction



function [z,s]=cplxroot(n,m)
//cplxroot(n,m,T,A,leg,flags,ebox)
//CPLXROOT Riemann surface for the n-th root.
//       CPLXROOT(n) renders the Riemann surface for the n-th root.
//       CPLXROOT, by itself, renders the Riemann surface for the cube root.
//       CPLXROOT(n,m) uses an m-by-m grid.  Default m = 20.
// Use polar coordinates, (r,theta).
// Cover the unit disc n times.
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs  < 1, n = 3; end
if rhs  < 2, m = 20; end
r = (0:m)'/m;
theta = - %pi*(-n*m:n*m)/m;
z = r * exp(%i*theta);
s = r.^(1/n) * exp(%i*theta/n);
endfunction

