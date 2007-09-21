// ==========================================
// Demonstrate animation based on the dynamic of a chain
// Copyright INRIA
// ==========================================


//Read the precomputed data of the chain evolution
path=get_absolute_file_path('anim6.sce');
yt=read(path+'yt.wrt',20,201);  //load data

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

//The initial chain position definition 
//-------------------------------------
angles=yt(1:$/2,1);

//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------

r=ones(angles); //length of each link
//coordinates of the links boundaries
x=[0;cumsum(2*r.*cos(angles))];
y=[0;cumsum(2*r.*sin(angles))];
n1=size(x,'*');
   
//set the axes boundary
curAxes=gca();
curAxes.data_bounds=2*sum(r)*[-1,-1;1,0.1];
title("N link pendulum movement","fontsize",3)

colors=1:n1;colors(8)=n1+1;
xpolys([x(1:($-1)),x(2:$)]',[y(1:($-1)),y(2:$)]',colors);
e = gce(); //compound with 10 polylines as children
e.children(:).thickness = 4;  

//animation loop
//--------------
//use realtime to slow down the loop
realtimeinit(0.04)//set time step (0.04 seconds)  and date reference
for j=1:size(yt,2)
  realtime(j)//wait till date 0.04*i seconds
  drawlater();
  angles=yt(1:$/2,j);
  r=ones(angles); //length of each link
  
  x=[0;cumsum(2*r.*cos(angles))];
  y=[0;cumsum(2*r.*sin(angles))];
  n=size(r,'*');
  for i = 1:n   
    e.children(n+1-i).data = [x(i),  y(i); x(i+1),y(i+1)];
  end
  drawnow();show_pixmap()
end
// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;
