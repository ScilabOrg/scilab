///////////////////////////////////////////////////////////////////
// Part II, Slanted Cylinder
///////////////////////////////////////////////////////////////////

function Yd=traj(tt,Y,surface)
  [p,q,r,s,t]=surface(Y(1),Y(3))
  w=-(r*Y(2)^2+2*s*Y(2)*Y(4)+t*Y(4)^2+9.81)/(1+p^2+q^2)
  Yd=[Y(2)
      p*w
      Y(4)
      q*w]
endfunction

function [p,q,r,s,t]=SlantedCylinder(x,y)
//z=f(x,y)=sqrt(1-x^2)-ay;
//
//p=df/dx
//q=df/dy
//r=df/dxx
//t=df/dyy
//s=df/dxy
  a=-tan(g_pente/180*%pi);
  z=sqrt(1-x^2)
  p=x/z;
  q=a //pente
  r=1/(z^3)
  s=0
  t=0
endfunction

function draw_cylinder(orig)
  a = -tan(g_pente/180*%pi);
  xx=-1:0.1:1;
  yy=(0:1:14)';
if orig then
  clf(); curFig = gcf();
  curFig.figure_position = [350 50];
  toolbar(curFig.figure_id,"off");
  curFig.pixmap = 'on';
  Z=-sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
  xselect();
  plot3d(xx,yy,Z');
  show_pixmap();
else
  xselect();
  drawlater();
  Z=-sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
  e = gce();
  e.data.z = Z';
  curAxe = gca();
  curAxe.tight_limits = "on";
  curAxe.data_bounds(1,3)=min(Z);
  drawnow();show_pixmap();
end
endfunction

function draw_point(orig)
  x = g_x;
  y = g_y;
  a = -tan(g_pente*%pi/180);
  r = 0.1;
  z=-sqrt(1-x^2)+a*y+r;
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

function draw_speed(orig)
  x = g_x;
  y = g_y;
  dx1 = g_V*cos(g_Vdir*%pi/180);
  dy1 = g_V*sin(g_Vdir*%pi/180);
  drawlater();
  if orig then
  param3d([x,x+dx1],[y,y+dy1],[0,0]);    
  e = gce(); e.thickness = 1; e.foreground = 5;
  e.polyline_style = 4;e.arrow_size_factor = 4;
  curAxe = gca(); 
  curAxe.data_bounds(1,1)=-1;
  curAxe.data_bounds(1,2)=0;
  curAxe.data_bounds(2,1)=1;
  curAxe.data_bounds(2,3)=0;
  else
  curAxe = gca(); 
  curAxe.data_bounds(1,1)=-1;
  curAxe.data_bounds(1,2)=0;
  curAxe.data_bounds(2,1)=1;
  curAxe.data_bounds(2,3)=0;
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

function Y = calculate_traj(g_x,g_y,g_V,g_Vdir)
  Y0(1)=g_x;//x
  Y0(2)=g_V*cos(g_Vdir*%pi/180);//v_x
  Y0(3)=g_y;//y
  Y0(4)=g_V*sin(g_Vdir*%pi/180);//v_y
  
  t=0:0.01:10; //1001 track points

  Y=ode(Y0,t(1),t,1d-10,1.D-10,list(traj,SlantedCylinder));//traj d'ecoulement

endfunction

function draw_bille(Y)
  g_tx=Y(1,:)';
  g_ty=Y(3,:)';
  g_tz=-sqrt(1-g_tx^2-g_ty^2);
endfunction

function execylinder()
    global g_pente; g_pente = 14;
    global g_x g_y; g_x = 0.6; g_y = 0.7;
    global g_V g_Vdir; g_V = 1; g_Vdir = 180;
    global go_on eff; go_on = %T; eff = %F;
    global g_alpha g_theta; g_alpha = 35; g_theta = 45;//observation point
    
    if go_on then draw_cylinder(%T);end
    if go_on then [g_pente,go_on]=pente(); end// set the pente
    if go_on then draw_point(%T); end
    if go_on then [g_x,g_y,go_on]=position(); end// set the initial point
    if go_on then draw_speed(%T); end
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
    if go_on then tra = calculate_traj(g_x,g_y,g_V,g_Vdir); end
    if findobj('tag','message')~=[] then close(f_msg); end
    if go_on then draw_traj_interface(tra); end
    
    clearglobal g_pente;
    clearglobal g_x g_y;
    clearglobal g_Vx g_Vy;
    clearglobal go_on eff;
    clearglobal g_alpha g_theta;
endfunction

