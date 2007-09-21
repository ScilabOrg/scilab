m=1; g=10; l=1;

theta0=0.2; phi0=%pi/4;
x0=l*[sin(phi0)*cos(theta0);..
      cos(phi0)*cos(theta0);sin(theta0)];

// index 1 with stabilization

u0=[1;-1;0];
deff("r=ff(l)","r=ggpp(x0,u0,l)");
lambda0=fsolve(0,ff);
ud0=-2*lambda0/m*x0-[0;0;g];
y0=[x0;u0;lambda0];
yd0=[u0;ud0;0];
t0=0; T=t0:0.01:15;

info=list([],0,[],[],[],0,0);
atol=[0.0001;0.0001;0.0001;0.001;0.001;0.001;1];
rtol=atol;

alfa=20; Beta=20;

y1s=dassl([y0,yd0],t0,T,rtol,atol,index1s,info);

x1s=y1s(2:4,:);

norm(x1s(:,$),2)

clf()
param3d(x1s(1,:),x1s(2,:),x1s(3,:))
a=gca();
a.title.text='spherical pendulum, index 1 with stabilization';
a.title.font_size=3;
