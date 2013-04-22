// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab maxtrix functions

pi=%pi;
i=%i;
e=%e;

// matdsr  matdsc
a=rand(4,4);ac=a+i*rand(4,4);t=a*a';tc=ac*ac';
tc=triu(tc,1)+triu(tc,1)'+diag(real(diag(tc)));
tc=tc/max(abs(spec(tc))+1);

// matrix functions
s=sqrtm(t);
assert_checktrue(norm(t-s*s,1) < 500*%eps);
s=logm(t);
assert_checktrue(norm(t-expm(s)) < 500*%eps);
s=sqrtm(tc);
assert_checktrue(norm(tc-s*s,1) < 500*%eps);
s=expm(tc);s=triu(s,1)+triu(s,1)'+diag(real(diag(s)));
assert_checktrue(norm(logm(s)-tc,1)< 500*%eps);
assert_checktrue(norm(sinm(t)**2+cosm(t)**2-eye(),1) < 50*%eps);
assert_checktrue(norm(sinm(tc)**2+cosm(tc)**2-eye(),1) < 50*%eps);
//poly and root
p=rand(5,1);pc=p+i*rand(5,1);x=poly(0,'x');
assert_checktrue(norm(gsort(p )-gsort(real(roots(poly(p,'x'))))) < 1000*%eps);
assert_checktrue(norm(gsort(imag(pc))-gsort(real(roots(poly(imag(pc),'x'))))) < 10000*%eps);
//**
assert_checktrue(norm(t^(-1)-inv(t),1) < 200*%eps);
assert_checktrue(norm(tc**(-1)-inv(tc),1) < 200*%eps);
x=rand()+i*rand();
assert_checktrue(norm((t**x)*(t^(-x))-eye(),1) < 200*%eps);
assert_checktrue(norm((tc**x)*(tc**(-x))-eye(),1) < 200*%eps);
//element op
assert_checktrue(norm(sin([0 pi/2 pi 3*pi/2])-[0 1 0 -1],1) < 10*%eps);
assert_checktrue(norm(cos([0 pi/2 pi 3*pi/2])-[1 0 -1 0],1) < 10*%eps);
assert_checktrue(norm(log([e 1/e e**2])-[1 -1 2],1) < 10*%eps);
assert_checktrue(norm(exp([1 -1 2])-[e 1/e e**2],1) < 10*%eps);
assert_checktrue(norm(sqrt([1 -1 4])-[1 i 2],1) < 10*%eps);
assert_checktrue(norm(sin(x)-(exp(i*x)-exp(-i*x))/(2*i),1) < 10*%eps);
assert_checktrue(norm(cos(x)-(exp(i*x)+exp(-i*x))/2,1) < 10*%eps);
assert_checktrue(norm(sqrt(x)^2-x,1) < 10*%eps);
assert_checktrue(norm(log(exp(x))-x,1) < 10*%eps);
// triu
z=poly(0,'z');a=[z 1 -z+1  8;z*z 10*z 5 -z;3 7 z+1 -1-z];
[m,n]=size(a);mn=min([m n]);a1=a;l=1;
//
for dg=-(m-1):0,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   for k=1:l,a1(-dg+k,k)=0;end,l=min([l+1,mn]);end
for dg=1:n,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a';a1=a;[m,n]=size(a);mn=min([m,n]);l=1;
for dg=-(m-1):0,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   for k=1:l,a1(-dg+k,k)=0;end,l=min([l+1,mn]);end
for dg=1:n,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a+%i*[1 2 3 4;5 6 7 8;9 10 11 12]';
[m,n]=size(a);mn=min([m n]);a1=a;l=1;
//
for dg=-(m-1):0,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   for k=1:l,a1(-dg+k,k)=0;end,l=min([l+1,mn]);end
for dg=1:n,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a';a1=a;[m,n]=size(a);mn=min([m,n]);l=1;
for dg=-(m-1):0,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   for k=1:l,a1(-dg+k,k)=0;end,l=min([l+1,mn]);end
for dg=1:n,
   assert_checktrue(norm(coeff(triu(a,dg)-a1),1)<%eps);
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
