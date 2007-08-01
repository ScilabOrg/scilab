// Copyright INRIA
mode(-1);

//display the diagram
x=[5,10,20,40,50,70,80,90];xmin=-10;xmax=100;
y=[22,28,30,32];ymin=12;ymax=40;

xx=[xmin,xmin,x([1 2 2 7 4 6 3 4 5 6 3 3 5 5]);xmax,xmax,x([3,2,7,7,5,8,3,4,5,6,4,4,6,6])];
yy=[ymin,ymax,y([3,1,1,1,3,3,2,2,2,2,2,4,2,4]);ymin,ymax,y([3,3,1,3,3,3,4,4,4,4,2,4,2,4])];
    
xset("window",0);xbasc();xselect();
plot2d(xx,yy,ones(1,16),'022');
xstring(28,30,'K');xstring(56,30,'Plant');xstring(12,28.80,'-');
xtitle('PLANT   and   CONTROLLER')

path=get_absolute_file_path('lqg.dem');
s=poly(0,'s');z=poly(0,'z');
