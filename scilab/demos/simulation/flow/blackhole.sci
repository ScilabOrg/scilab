///////////////////////////////////////////////////////////////////
// Part III, black hole,z=f(x,y)=-(1/sqrt(x^2+y^2))
///////////////////////////////////////////////////////////////////
function Yd=traj(tt,Y,surface)
  [p,q,r,s,t]=surface(Y(1),Y(3))
  w=-(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2)
  Yd=[Y(2)
      p*w
      Y(4)
      q*w]
endfunction

function  [p,q,r,s,t]=blackhole(x,y)
//z=f(x,y)=-(1/sqrt(x^2+y^2));
//
//p=df/dx
//q=df/dy
//r=df/dxx
//t=df/dyy
//s=df/dxy
  z=-(1/sqrt(x^2+y^2));
  p=-x*z^3;
  q=-y*z^3;
  r=z^3*(3*x^2*z^2-1);
  s=3*x*y*z^5;
  t=z^3*(3*y^2*z^2-1);
endfunction

function draw_surface()
    r=1; orig=[0 0 0]; 
    xx = -1.85:0.1:1.85;
    yy = (-1.85:0.1:1.85)';
    zt = sqrt((ones(yy)*(xx^2)+(yy^2)*ones(xx)));
    zz = -1*ones(zt)./zt;
    clf();
    curFig=gcf();curFig.pixmap='on';xselect();
    curFig.figure_position = [350 50];
    toolbar(curFig.figure_id, "off");
    drawlater();
    mesh(xx,yy,zz);
    e=gce();
    e.color_mode = 0;
    drawnow();
    show_pixmap();
endfunction

function draw_point(g_r,g_t,orig)
  
  x = g_r*cos(g_t*%pi/180);
  y = g_r*sin(g_t*%pi/180);
  r = 0.05;
  z=-(ones(x)./sqrt(x^2+y^2))+r;
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
  dx1 = g_V*cos(g_Vdir*%pi/180)/2;
  dy1 = g_V*sin(g_Vdir*%pi/180)/2;
  drawlater();
  if orig then
  param3d([x,x+dx1],[y,y+dy1],[0,0]);    
  e = gce(); e.thickness = 1; e.foreground = 5;
  e.polyline_style = 4;e.arrow_size_factor = 4;
  curAxe = gca(); curAxe.data_bounds = [-2 -2 -16; 2 2 0];
  else
  curAxe = gca(); curAxe.data_bounds = [-2 -2 -16; 2 2 0];
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
  
  t=0:0.02:10; //501 track points

  Y=ode(Y0,t(1),t,1d-10,1.D-10,list(traj,blackhole));//traj d'ecoulement

endfunction

function exeblackhole()
    global g_r g_t; g_r = 1.1; g_t = 41;//initial point
    global g_V g_Vdir; g_V = 1.65; g_Vdir = 165;//initial speed
    global go_on eff; go_on = %T; eff = %F;
    global g_alpha g_theta; g_alpha = 35; g_theta = 45;//observation point
    if go_on then draw_surface(); end
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
