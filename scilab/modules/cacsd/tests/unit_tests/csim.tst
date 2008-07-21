// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=0;b=1;c=2;
t=0:0.1:1;
sl=syslin('c',a,b,c);

//step
y=csim('step',t,sl);
if size(y,1)<>1 then pause,end
y=csim('step',[],sl);
if size(y,2)<>0 then pause,end
[y1,x]=csim('step',[],sl);
if size(y1,2)<>0 then pause,end
if size(x,2)<>0 then pause,end

y=csim('step',t,sl);
if norm(y-c*t,1)>1.d-15 then pause,end
y=csim('step',t',sl);
if norm(y-c*t,1)>1.d-15 then pause,end

[y1,x]=csim('step',t,sl);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim('step',t,sl,0);
if norm(y-c*t,1)>1.d-15 then pause,end

[y1,x]=csim('step',t,sl,0);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim('step',t,sl,1);
if norm(y-c*t,1)>1.d-14 then pause,end

[y1,x]=csim('step',t,sl,1);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim('step',t,sl,0,[10*%eps %eps]);
if norm(y-c*t,1)>1.d-14 then pause,end

[y1,x]=csim('step',t,sl,1,[10*%eps %eps]);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end


//impulse
y=csim('impulse',t,sl);
if size(y,1)<>1 then pause,end
y=csim('impulse',[],sl);
if size(y,2)<>0 then pause,end
[y1,x]=csim('impulse',[],sl);
if size(y1,2)<>0 then pause,end
if size(x,2)<>0 then pause,end

w=ones(t)-eye(t);
y=csim('impulse',t,sl);
if norm(y-c*w,1)>1.d-15 then pause,end
y=csim('impulse',t',sl);
if norm(y-c*w,1)>1.d-15 then pause,end

[y1,x]=csim('impulse',t,sl);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*w,1)<>0 then pause,end

y=csim('impulse',t,sl,0);
if norm(y-c*w,1)>1.d-15 then pause,end

[y1,x]=csim('impulse',t,sl,0);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*w,1)<>0 then pause,end

y=csim('impulse',t,sl,1);
if norm(y-c*w,1)>1.d-14 then pause,end

[y1,x]=csim('impulse',t,sl,1);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim('impulse',t,sl,0,[10*%eps %eps]);
if norm(y-c*w,1)>1.d-14 then pause,end

[y1,x]=csim('impulse',t,sl,1,[10*%eps %eps]);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

//function
function y=step(t),if t<0 then y=0,else y=1,end,endfunction
y=csim(step,t,sl);
if size(y,1)<>1 then pause,end
y=csim(step,[],sl);
if size(y,2)<>0 then pause,end
[y1,x]=csim(step,[],sl);
if size(y1,2)<>0 then pause,end
if size(x,2)<>0 then pause,end

y=csim(step,t,sl);
if norm(y-c*t,1)>1.d-15 then pause,end
y=csim(step,t',sl);
if norm(y-c*t,1)>1.d-15 then pause,end

[y1,x]=csim(step,t,sl);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim(step,t,sl,0);
if norm(y-c*t,1)>1.d-15 then pause,end

[y1,x]=csim(step,t,sl,0);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim(step,t,sl,1);
if norm(y-c*t-2,1)>1.d-14 then pause,end

[y1,x]=csim(step,t,sl,1);
if norm(y1-y,1)<>0 then pause,end
if norm(y1-c*x,1)<>0 then pause,end

y=csim(step,t,sl,0,[10*%eps %eps]);
if norm(y-c*t,1)>1.d-14 then pause,end

//with transfer function
y=csim(step,t,ss2tf(sl));
if norm(y-c*t,1)>1.d-14 then pause,end

//with parameter
function y=step(t,t0),if t<t0 then y=0,else y=1,end,endfunction

y=csim(list(step,0),t,sl);
if norm(y-c*t,1)>1.d-14 then pause,end

y=csim(list(step,0),t,ss2tf(sl));
if norm(y-c*t,1)>1.d-14 then pause,end
