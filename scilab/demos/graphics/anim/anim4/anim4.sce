// ==========================================
// Demonstrate animation based on the rotation of a 3D contour plot
// Copyright INRIA
// ==========================================
mode(0);


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


//The surface definition 
//----------------------
x=linspace(-%pi,%pi,50);
y=x;
Z=sin(x)'*cos(y);

//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------

contour(x,y,Z,10,35,45,'X@Y@Z',[0,2,4])
title("rotation of a 3d contour","fontsize",3)
curAxe = gca();	
//Set the evolution of the view angle  Alpha
//---------------------------------------------------
A=35:80;
//animation loop
//--------------
//use realtime to slow down the loop
realtimeinit(0.05);//set time step (0.05 seconds)  and date reference
for i=1:size(A,'*')
  realtime(i)//wait till date 0.05*i seconds
  curAxe.rotation_angles = [45,A(i)];
  show_pixmap(); //send  buffer to screen
end

// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;

