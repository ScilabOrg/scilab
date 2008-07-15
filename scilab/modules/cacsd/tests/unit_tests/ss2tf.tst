// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=0.5;b=1;c=2;
sl=syslin('c',a,b,c);
h1=ss2tf(sl);
if h1.num<>2 then pause,end
if h1.den<>poly([-0.5 1],'s','c') then pause,end
d=1;
sl=syslin('c',a,b,c,d);
h2=ss2tf(sl);
if h2<>h1+1 then pause,end

b=[b b];
sl=syslin('c',a,b,c);
h2=ss2tf(sl);
if or(h2<>[h1 h1]) then pause,end


a=[0.9,0,0.6,-1.4,-4.2;
   0.2,0.1,-0.2,0.5,0.6;
   -4.3,0,2.2,0,2.4;
   -3.7,-0.5,2.4,-0.6,2.7;
   6.4,0.1,-4,-0.5,-4];

b=[-0.1,-0.1,0;
   0,0,0.1;
   -0.1,0.2,-0.1;
   0.2,0.2,-0.6;
   0.2,-0.1,0.1];
 
c=[2,7,-2,5,1
   0,-1,3,0,2];
d=[1,0,0
   0,0,0];

sl=syslin('c',a,b,c,d);

h=ss2tf(sl)
if or(degree(h.den)<>5) then pause,end
if or(degree(h.num)<>[5,4,4;4,4,4]) then pause,end

r1=spec(a);R1=gsort([real(r1) imag(r1)],'lr','i');
r2=roots(h.den(1,1));R2=gsort([real(r2) imag(r2)],'lr','i');
if norm(R2-R1,'inf')>1d-14 then pause,end


