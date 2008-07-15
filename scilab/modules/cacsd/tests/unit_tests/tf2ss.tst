// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================



s=%s;
n=1;d=1+3*s;
h=syslin('c',n./d);
sl=tf2ss(h);
if sl.A<>-1/3 then pause,end
if sl.B*sl.C<>1/3 then pause,end
if sl.D<>0 then pause,end
if sl.X0<>0 then pause,end
if sl.dt<>'c' then pause,end

h=syslin('d',n./d);
sl=tf2ss(h);
if sl.A<>-1/3 then pause,end
if sl.B*sl.C<>1/3 then pause,end
if sl.D<>0 then pause,end
if sl.X0<>0 then pause,end
if sl.dt<>'d' then pause,end

h1=[h h];
sl1=tf2ss(h1);
if abs(sl1.A+1/3)>1d-15 then pause,end
if norm(sl1.B*sl1.C-1/3,1)>1d-15  then pause,end
if or(sl1.D<>[0 0]) then pause,end
if sl1.X0<>0 then pause,end
if sl1.dt<>'d' then pause,end


n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];

h=syslin('c',n./d);
sl=tf2ss(h); 
cden=lcm(d);cden=cden/coeff(cden,degree(cden))
if norm(coeff(cden-poly(sl.A,'s')),1)>1d-10 then pause,end



e=h-ss2tf(sl);
if norm(coeff(e.num))>>1d-10 then pause,end
 
n=(1+s);d=(1+3*s)*(0.9999+s);
h=syslin('c',n./d);
sl=tf2ss(h,0.01);
if size(sl.A,'*')<>1 then pause,end
