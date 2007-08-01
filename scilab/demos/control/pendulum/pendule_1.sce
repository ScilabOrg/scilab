mode(-1)
clc;
xselect();wdim=xget('wdim')
//mode(1)

 dpnd()
//
// equations 
//----------
//state =[x x' theta theta']  
//
 mb=0.1;mc=1;l=0.3;m=4*mc+mb; //constants
//
demo_message('SCI/demos/control/pendulum/pendule_2.sce')
//
 y0=[0;0;0.1;0];
 y=ode(y0,0,0.03*(1:180),ivpd);

 P=initialize_display(y0(1),y0(3));
 for k=1:size(y,2), set_pendulum(P,y(1,k),y(3,k));end

//
demo_message('SCI/demos/control/pendulum/pendule_3.sce')

//
mode(1)
 x0=[0;0;0;0];u0=0;
 [f,g,h,j]=lin(pendu,x0,u0);
 pe=syslin('c',f,g,h,j);
 ssprint(pe)
mode(-1)
 
//
demo_message('SCI/demos/control/pendulum/pendule_4.sce')

//
mode(1)
 f1=[0 1        0             0
    0 0    -3*mb*9.81/m         0
    0 0        0             1
    0 0  6*(mb+mc)*9.81/(m*l)   0];
 g1=[0 ; 4/m ; 0 ; -6/(m*l)];
 h1=[1 0 0 0
     0 0 1 0];
 err=norm(f-f1,1)+norm(g-g1,1)+norm(h-h1,1)+norm(j,1)
 mode(-1)
 
demo_message('SCI/demos/control/pendulum/pendule_5.sce')
//---------
mode(1)
 spec(f) //stability (unstable system !)
 n=contr(f,g) //controlability

 //observability
 m1=contr(f',h(1,:)') 
 [m2,t]=contr(f',h(2,:)')
 mode(-1)
//
demo_message('SCI/demos/control/pendulum/pendule_6.sce')

//-------------------------------------------------
//
//pole placement technique
//only x and theta are observed  : contruction of an observer
//to estimate the state : z'=(f-k*h)*z+k*y+g*u
//
 to=0.1;  //
 k=ppol(f',h',-ones(4,1)/to)'  //observer gain
//
//verification
//
// norm( poly(f-k*h,'z')-poly(-ones(4,1)/to,'z'))
//
 kr=ppol(f,g,-ones(4,1)/to)  //compensator gain
 
//
demo_message('SCI/demos/control/pendulum/pendule_7.sce')

//---------------------------------------------
//
//state: [x x-z]
//
mode(1)
 ft=[f-g*kr            -g*kr
      0*f               f-k*h];
 gt=[g;0*g];
 ht=[h,0*h];
 pr=syslin('c',ft,gt,ht);

// closed loop dynamics:
 spec(pr(2))
//transfer matrix representation
 hr=clean(ss2tf(pr),1.d-10)
 
 
 //frequency analysis
 clf()
 black(pr,0.01,100,['position','theta'])
 g_margin(pr(1,1))
 mode(-1)
 
//
demo_message('SCI/demos/control/pendulum/pendule_8.sce')

//---------------
//
mode(1)
 t=to/5;
 prd=dscr(pr,t);
 spec(prd(2))
mode(-1)
//
demo_message('SCI/demos/control/pendulum/pendule_9.sce')

//-----------------
//
mode(1)
 x0=[0;0;0;0;0;0;0;0];
 u(1,180)=0;u(1,1)=1;
 y=flts(u,prd,x0);

 draw1()
mode(-1)
//
demo_message('SCI/demos/control/pendulum/pendule_10.sce')
;
//--------------------------------------
//
//simulation
//
mode(1)
 t0=0;t1=t*(1:125);
 x0=[0 0 0.4 0   0 0 0 0]';   //
 yd=ode(x0,t0,t1,regu);
 draw2()
mode(-1) 
 demo_message('SCI/demos/control/pendulum/pendule_11.sce')
