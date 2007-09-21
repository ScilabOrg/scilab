function demo_datafit()
  //demo_help demo_datafit
  
  //create the experimental data
  X=[];Y=[];
  a=34;b=12;c=14;
  for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
  Z=[Y;X];
  //show the data points
  clf();
  curFig = gcf();
  fs = curFig.figure_style;
  curFig.figure_style = "new";
  curFig.figure_position = [600,16];  
  curFig.figure_size = [600*0.9,400*0.9];
  pix = curFig.pixmap;
  curFig.pixmap = "on";
  xselect();
  curAxe = gca();
  curAxe.title.font_size = 3;
  curAxe.title.text ="non linear data fitting";
  plot2d(X',Y',style=-1,leg='Experimental data');
  show_pixmap();
  
  realtimeinit(0.1);for k=1:20,realtime(k),end
  // solve the non linear data fitting
  [p,err]=datafit(G,Z,[3;5;10])
  // show the fitting curve
  plot2d(X',FF(X)',[5,2],'002','Fitting function');
  show_pixmap();
 realtimeinit(0.1);for k=1:30,realtime(k),end
  set('old_style','off')
  
  curFig.pixmap = pix;
  curFig.figure_style = fs;
endfunction

function y=FF(x)
//parametric function model
  y=a*(x-b)+c*x.*x,
endfunction

function e=G(p,z)
//datafit external computes the error
  a=p(1),
  b=p(2),
  c=p(3),
  y=z(1),
  x=z(2),
  e=y-FF(x)
endfunction
