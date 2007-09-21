// ==========================================
// Demonstrate animation based on the dynamic of a chain
// Copyright INRIA
// ==========================================

curFig = gcf();clf();  // erase window
xselect(); //raise the graphic window

//turn off toolbar
//----------------
id=curFig.figure_id;
tb=toolbar(id); //preserve setting
toolbar(id,"off");

// set double buffer mode to avoid blinking animation
//---------------------------------------------------
pix = curFig.pixmap; //preserve old setting
curFig.pixmap = "on";

//Create the data
//---------------

n=60;
U = linspace(0,3*%pi,n);
v = linspace(0,2*%pi,20);

u = U(1:20);
X = (cos(u).*u)'*(1+cos(v)/2);
Y = (u/2)'*sin(v);
Z = (sin(u).*u)'*(1+cos(v)/2);

[xx,yy,zz]=nf3d(X,Y,Z);//build facets

//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------

plot3d(xx,yy,zz) //creates a Fac3d entity
e = gce();
data=e.data;
title("shell","fontsize",3)

curAxe=gca();
// set 3D boundaries
curAxe.data_bounds=[-15 -5 -10;
		     10  5  12];
//set view angles
curAxe.rotation_angles=[152 62];

//animation loop
//--------------
//use realtime to slow down the loop
K=20:4:n;
realtimeinit(0.1);//set time step (0.1 seconds)  and date reference
for k=2:size(K,'*')
  realtime(k);
  //compute more facets
  u=U(K(k-1):K(k));
  X= (cos(u).*u)'*(1+cos(v)/2);
  Y= (u/2)'*sin(v);
  Z= (sin(u).*u)'*(1+cos(v)/2);
  [xx,yy,zz]=nf3d(X,Y,Z);//build facets
  //append new facets to the data data structure
  data.x=[data.x xx];
  data.y=[data.y yy];
  data.z=[data.z zz];
  
  e.data=data;// update the Fac3d entity
  show_pixmap();//send  buffer to screen
end
	
// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;

