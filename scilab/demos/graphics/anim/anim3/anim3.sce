// ==========================================
// Demonstrate animation based on the rotation of Lorentz curve
// Copyright INRIA
// ==========================================

mode(0)


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

//compute lorenz differential eqn solution
//---------------------------------------------------
  
y=ode(1.e-8*[1;1;1],0,0:0.005:50,'loren');
[n1,n2]=size(y);
wid = size(curFig.color_map,1); //number of colors

//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------
// we split the full trajectory in a set of polylines, each of them
// having a different color
drawlater(); //disable automatic redrawing
for j=1:wid;//loop on the colors
  tt= ((j-1)*(n2/wid)+1):(j*(n2/wid));
  param3d(y(1,tt),y(2,tt),y(3,tt));
  e = gce(); //handle on the just created 3D polyline
  e.foreground = j;//set its color
end
title("Lorentz curve","fontsize",3);

curAxe = gca();
curAxe.box = "off";
curAxe.axes_visible = "off";
curAxe.grid= [12,12,12];
drawnow()

//Set the evolution of the view angle  Alpha
//---------------------------------------------------
A=35:5:160;

//animation loop
//--------------
//use realtime to slow down the loop
realtimeinit(0.1);//set time step (0.1 seconds)  and date reference
for i=1:max(size(A))
  realtime(i) //wait till date 0.1*i seconds
  curAxe.rotation_angles = [45,A(i)];
  show_pixmap();  //send  buffer to screen
end
  

// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;

