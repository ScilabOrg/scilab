// Copyright Enpc 

getf('SCI/demos/demos_lib/demo_run.sci');

function []=str_1()
expr=['t=(0:0.1:6*%pi);';
      'plot2d(t'',sin(t)'');';
      'xtitle(''plot2d and xgrid '',''t'',''sin(t)'');';
      'current_axe=gca(); current_axe.title.font_size=3;';
      'xgrid();'];
      demo_run('',expr);
endfunction      

function []=str_2()
expr=['plot2d1(''enl'',1,(1:10:10000)'');';
         'xtitle(''plot2d1 log scale'',''t'',''y  log scale'');';
         'current_axe=gca(); current_axe.title.font_size=3;';
	 'xgrid(3);'];
	 demo_run('',expr);
endfunction

function []=str_3()
expr=['n=32-1;t=(0:n)/n;';
     'u=sin(80*%pi*t)+sin(100*%pi*t);';
     'plot2d3(''enn'',1,abs(fft(u,-1))''/n);';
     'xtitle(''plot2d3 (vbar plot)'',''t'',''f(t)'');';
     'current_axe=gca(); current_axe.title.font_size=3;'];
     demo_run('',expr);
endfunction

function []=str_4()
expr=['rand(''normal'');';
     'v=(1:20)+(1:20).*rand(1,20);';
     'plot2d1(''enn'',1,v'');';
     'plot2d1(''enn'',1,(1:20)'',[2,-2],''100'',''estimated'');';
     'xtitle(''plot2d1 '','' '','' '');';
     'current_axe=gca(); current_axe.title.font_size=3;'];
     demo_run('',expr);
endfunction

function []=str_5()
expr=['histplot()';
      'current_axe=gca(); current_axe.title.font_size=3;'];
    demo_run('',expr);
endfunction

function []=str_6()
expr=['fplot2d();';
     'xtitle(''fplot2d : f given by external '',''x '',''f(x) '');';
     'current_axe=gca(); current_axe.title.font_size=3;'];
     demo_run('',expr);
endfunction

function []=str_7()
expr=['param3d();';
      'xtitle(''param3d : parametric curves in R3'','' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_8()
expr=['t=-50*%pi:0.1:50*%pi;';
      'x=t.*sin(t);y=t.*cos(t);z=t.*abs(t)/(50*%pi);';
      'param3d(x,y,z,45,60);';
      'Title=[''param3d : parametric curves in R3 (t.sin(t),t.cos(t),t.|t|/50.%pi)''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_9()
expr=['plot3d();';
      'Title=[''plot3d : z=sin(x)*cos(y)''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_10()
expr=['fplot3d();';
      'Title=[''fplot3d : z=f(x,y), f external''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_11()
expr=['plot3d1();';
      'Title=[''plot3d1 : z=sin(x)*cos(y)''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_12()
expr=['fplot3d1();';
      'Title=[''fplot3d : z=f(x,y), f external''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_13()
expr=['contour();';
	   'Title=''contour'';';
	   'xtitle(Title,'' '','' '');';
	   'current_axe=gca(); current_axe.title.font_size=3;'];
	   demo_run('',expr);
endfunction

function []=str_14()
expr=['fcontour();'; 
	   'Title=[''fcontour''];';
	   'xtitle(Title,'' '','' '');';
	   'current_axe=gca(); current_axe.title.font_size=3;'];
	   demo_run('',expr);
endfunction

function []=str_15()
expr=['contourf();'; 
	   'Title=[''contourf''];';
	   'xtitle(Title,'' '','' '');';
	   'current_axe=gca(); current_axe.title.font_size=3;'];
	   demo_run('',expr);
endfunction

function []=str_16()
expr=['champ();';
      'Title=[''champ ''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_17()
expr=['fchamp();';
      'Title=[''fchamp ''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_18()
expr=['grayplot();';
      'Title=[''grayplot ''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_19()
expr=['fgrayplot();';
      'Title=[''fgrayplot ''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_20
expr=['errbar();';
      'Title=[''errbar ''];';
      'xtitle(Title,'' '','' '');';
      'current_axe=gca(); current_axe.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_21()
expr=['chart();';
      'current_axe=gca(); current_axe.title.font_size=3;'];
    demo_run('',expr);
endfunction

function []=str_22()
expr=['zgrid();';
      'current_axe=gca(); current_axe.title.font_size=3;'];
    demo_run('',expr);
endfunction

function []=str_23()
expr=['r=(%pi):-0.01:0;';
	'x=r.*cos(10*r);';
	'y=r.*sin(10*r);';
	'';
	'deff(''[z]=Surf(x,y)'',''z=sin(x)*cos(y)'');';
	't=%pi*(-10:10)/10;';
	'';
	'// Plot the surface';
	'fplot3d(t,t,Surf,35,45,''X@Y@Z'',[19,2,3]);';
	'';
	'// Add 2d graphics on 3d graphic';
	'z=(sin(x).*cos(y));';
	'[x1,y1]=geom3d(x,y,z);';
	'xpoly(x1,y1,''lines'');';
	'';
	'// Adjust some plotting parameters';
	'BackgroundColorId = color(70,174,255);';
	'current_axe = gca();';
	'plot_3d = current_axe.children(2);';
	'plot_3d.hiddencolor = 32;';
	'polyline = current_axe.children(1)';;
	'polyline.foreground = 8;';
	'current_axe.rotation_angles = [70,47];';
	'current_axe.background = BackgroundColorId;';
	'';
	'// A second 2d graphics';
	'[x1,y1]=geom3d([0,0],[0,0],[5,0]);';
	'xsegs(x1,y1);';
	'xstring(x1(1),y1(1),''The point (0,0,0)'');';
	'Title=[''plot3d and use of xgeom''];';
	'xtitle(Title,'' '','' '');';
	'current_axe=gca(); current_axe.title.font_size=3;'];
	demo_run('',expr);
endfunction

function []=str_24()
expr=['t=%pi*(-10:10)/10;';
          'deff(''[z]=Surf(x,y)'',''z=sin(x)*cos(y)'');';
          'rect=[-%pi,%pi,-%pi,%pi,-5,1];';
          'z=feval(t,t,Surf);';
          'contour(t,t,z,10,35,45,''X@Y@Z'',[1,1,0],rect,-5);';
          'plot3d(t,t,z,35,45,''X@Y@Z'',[2,1,3],rect);';
          'Title=[''plot3d and contour 2''];';
          'xtitle(Title,'' '','' '');';
          'current_axe=gca(); current_axe.title.font_size=3;'];
          demo_run('',expr);
endfunction

function []=str_25()
expr=['t=%pi*(-10:10)/10;';
          'deff(''[z]=Surf(x,y)'',''z=sin(x)*cos(y)'');';
          'rect=[-%pi,%pi,-%pi,%pi,-1,1];';
          'z=feval(t,t,Surf);';
          'plot3d(t,t,z,35,45,''X@Y@Z'',[-1,1,3],rect);';
          'contour(t,t,z+0.1,10,35,45,''X@Y@Z'',[0,1,0],rect);';
          'Title=[''plot3d and contour 3''];';
          'xtitle(Title,'' '','' '');';
          'current_axe=gca(); current_axe.title.font_size=3;'];
          demo_run('',expr);
endfunction

function []=str_26()
expr=['t=%pi*(-10:10)/10;';
          'deff(''[z]=Surf(x,y)'',''z=sin(x)*cos(y)'');';
          'rect=[-%pi,%pi,-%pi,%pi,-1,1];';
          'z=feval(t,t,Surf);';
          'plot3d(t,t,z,35,45,''X@Y@Z'',[-20,1,3],rect);';
          'contour(t,t,z+0.1,10,35,45,''X@Y@Z'',[0,1,0],rect);';
          'Title=[''plot3d and contour 4''];';
          'xtitle(Title,'' '','' '');';
          'current_axe=gca(); current_axe.title.font_size=3;'];
          demo_run('',expr);
endfunction

function []=str_27()
expr=['exec(''SCI/demos/graphics/sd.sav'');';
          'set figure_style old';
          'plot2d();gr_menu(sd_1,1,1);';
          'Title=[''plot2d and gr_menu''];';
          'xtitle(Title,'' '','' '');'];
          demo_run('',expr);
endfunction

function []=str_28()
expr=['exec(''SCI/demos/graphics/sd.sav'');';
          'set figure_style old';
          'plot3d();gr_menu(sd_2,1,1);';
          'Title=[''plot3d and gr_menu''];';   
          'xtitle(Title,'' '','' '');'];
          demo_run('',expr);
endfunction

function []=str_29()
expr=['subplot(2,2,1);';
	'plot3d();';
	'subplot(2,2,2);';
	'plot2d();';
	'subplot(2,2,3);';
	'histplot();';
	'subplot(2,2,4);';
	'grayplot();'
	'a=gca();';
  'a.title.text=""subplot"";';
  'a.title.font_size=3;'];
	demo_run('',expr);
endfunction

function []=str_30()
expr=['fac3d();'
      'a=gca();'
      'a.title.text=""fac 3d"";'
      'a.title.font_size=3;'];
      demo_run('',expr);
endfunction

function []=str_31()
expr=['fac3d1();'
      'a=gca();'
      'a.title.text=""fac 3d1"";'
      'a.title.font_size=3;'];
      demo_run('',expr);
endfunction

x=90;
