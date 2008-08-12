// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')

//==========================================================================
//==============================    /         ============================== 
//==========================================================================
//function x=rdiv(A,B),x=slash(A,B),endfunction
function x=rdiv(A,B),x=A/B,endfunction
//scalar division
  
//-----Square matrix-----
n=5;  A=rand(n,n);b=rand(2,n+1);
if rdiv([],A) <>[] then pause,end
if execstr('rdiv(b,A)','errcatch')==0 then pause,end
//Small dimensions real
n=5;
b=rand(2,n);A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>200*%eps then pause,end
//Small dimensions complex
b=rand(2,n)+%i;A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

b=rand(2,n);A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

b=rand(2,n)+%i;A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

//Large dimensions real
n=50;
b=rand(2,n);A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end
//Small dimensions complex
b=rand(2,n)+%i;A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

b=rand(2,n);A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

b=rand(2,n)+%i;A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

//-----Rectangular matrix-----
n=5;m=3; A=rand(m,n);b=rand(2,n+1);
if rdiv([],A) <>[] then pause,end
if execstr('rdiv(b,A)','errcatch')==0 then pause,end

//Small dimensions real
n=5;m=3;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end

n=3;m=5;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
//Small dimensions complex
n=5;m=3;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
n=5;m=3;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end

n=3;m=5;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=3;m=5;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=3;m=5;
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

//LArge dimension real

n=50;m=30;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
//Large dimensions complex
n=50;m=30;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
n=50;m=30;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

