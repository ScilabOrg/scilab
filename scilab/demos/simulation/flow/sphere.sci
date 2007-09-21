///////////////////////////////////////////////////////////////////
// Part I, sphere
///////////////////////////////////////////////////////////////////

function Yd=traj(tt,Y,surface)
  [p,q,r,s,t]=surface(Y(1),Y(3))
  w=-(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2)
  Yd=[Y(2)
      p*w
      Y(4)
      q*w]
endfunction

function  [p,q,r,s,t]=sphere(x,y)
//z=f(x,y)=-sqrt(1-x^2-y^2)
//
//df/dx=-x/sqrt(1-x^2-y^2)
//df/dy=-y/sqrt(1-x^2-y^2)
//df/dxx=(y^2-1)/(1-x^2-y^2)^(3/2)
//df/dyy=(X^2-1)/(1-x^2-y^2)^(3/2)
//df/dxy=(-xy)/(1-x^2-y^2)^(3/2)
  z=-sqrt(1-(x^2+y^2));
  w=z^3
  p=-x/z;
  q=-y/z;
  r=(y^2-1)/w;
  s=(-x*y)/w;
  t=(x^2-1)/w;
endfunction

function [x,y,z]=sph(alp,tet)
  x=r*cos(alp).*cos(tet)+orig(1)*ones(tet)
  y=r*cos(alp).*sin(tet)+orig(2)*ones(tet)
  z=r*sin(alp)+orig(3)*ones(tet);
endfunction

function draw_sphere()
    r=1; orig=[0 0 0];
    [xx,yy,zz]=eval3dp(sph,linspace(-%pi/2,0,40),linspace(0,%pi*2,20));
    clf(); curFig = gcf();
    curFig.figure_position = [350 50];
    toolbar(curFig.figure_id,"off");
    curFig.pixmap = 'on';
    plot3d(xx,yy,zz)
    show_pixmap()
endfunction

function draw_point(g_r,g_t,orig)
  
  x = g_r*cos(g_t*%pi/180);
  y = g_r*sin(g_t*%pi/180);
  r = 0.05;
  z=-sqrt(1-x^2-y^2)+r;
  drawlater();
  if orig then
  param3d([x-r,x+r],[y,y],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
  param3d([x,x],[y-r,y+r],[z,z]);  e = gce(); e.thickness = 1; e.foreground = 5;
  param3d([x,x],[y,y],[z-r,z+r]);  e = gce(); e.thickness = 1; e.foreground = 5;
  else
  curAxe = gca();
  e1 = curAxe.children(1); e1.data = [x,y,z-r;x,y,z+r];
  e2 = curAxe.children(2); e2.data = [x,y-r,z;x,y+r,z];
  e3 = curAxe.children(3); e3.data = [x-r,y,z;x+r,y,z];
  end  
  drawnow();show_pixmap();
endfunction

function draw_speed(g_r,g_t,g_V,g_Vdir,orig)
  x = g_r*cos(g_t*%pi/180);
  y = g_r*sin(g_t*%pi/180);
  dx1 = g_V*cos(g_Vdir*%pi/180);
  dy1 = g_V*sin(g_Vdir*%pi/180);
  drawlater();
  if orig then
  param3d([x,x+dx1],[y,y+dy1],[0,0]);    
  e = gce(); e.thickness = 1; e.foreground = 5;
  e.polyline_style=4;e.arrow_size_factor=4;
  curAxe = gca(); curAxe.data_bounds = [-1 -1 -1; 1 1 0];
  else
  curAxe = gca(); curAxe.data_bounds = [-1 -1 -1; 1 1 0];
  e = curAxe.children(1); e.data = [x,y,0;x+dx1,y+dy1,0];
  end  
  drawnow();show_pixmap();

endfunction

function clear_ini()
  curAxe = gca();
  num = size(curAxe.children,'*');
  drawlater();
  for i=num:-1:1
    if (curAxe.children(i).type == "Polyline")
      delete(curAxe.children(i))
    end
  end
  drawnow();
  show_pixmap();
endfunction

function Y = calculate_traj(g_r,g_t,g_V,g_Vdir)
  Y0(1)=g_r*cos(g_t*%pi/180);//x
  Y0(2)=g_V*cos(g_Vdir*%pi/180);//v_x
  Y0(3)=g_r*sin(g_t*%pi/180);//y
  Y0(4)=g_V*sin(g_Vdir*%pi/180);//v_y
  
  t=0:0.01:15; //1501 track points

  Y=ode(Y0,t(1),t,1d-10,1.D-10,list(traj,sphere));//traj d'ecoulement

endfunction

function exesphere()
    global g_r g_t; g_r = 0.7; g_t = 250;//initial point
    global g_V g_Vdir; g_V = 1; g_Vdir = 180;//initial speed
    global go_on eff; go_on = %T; eff = %F;
    global g_alpha g_theta; g_alpha = 35; g_theta = 45;//observation point

    if go_on then draw_sphere(); end
    if go_on then draw_point(g_r,g_t,%T); end
    if go_on then [g_r,g_t,go_on]=position(); end// set the initial point
    if go_on then draw_speed(g_r,g_t,g_V,g_Vdir,%T); end
    if go_on then [g_V,g_Vdir,go_on]=speed(); end// set the initial speed
    if go_on then clear_ini(); end // clear the point and the vector of speed
    if go_on then 
	   f_msg = figure("Position",[30 50 300 200],...
                 "BackgroundColor",[0.9 0.9 0.9],...
    	            "Unit"    , "pixel",...
    	            "tag"     , "message");
	   title1 = uicontrol(f_msg, "Position"  , [25 80 250 30],...
  		 "Style"     , "text",...
  		 "String"    , "Generating Trajetory",...
  		 "BackgroundColor",[0.9 0.9 0.9],...
  		 "Fontsize"  , 20);
    end
    if go_on then tra = calculate_traj(g_r,g_t,g_V,g_Vdir); end
    if findobj('tag','message')~=[] then close(f_msg); end
    if go_on then draw_traj_interface(tra); end
    
    clearglobal g_r g_t;
    clearglobal g_V g_Vdir;
    clearglobal go_on eff;
    clearglobal g_alpha g_theta;
endfunction

