m1=1;m2=1;r1=1;r2=1;
// Copyright INRIA
g=9.8;
t0=0;
t=0:.1:1.5;
z0=[%pi/2;%pi/2;0;0];
getf('dpend.sci');
z=ode(z0,t0,t,dpend);
pp(z);
