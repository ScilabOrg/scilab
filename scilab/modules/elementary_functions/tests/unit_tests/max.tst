// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

Ntest=1
// test max(A) : A matrix
// maxi is compared with sort

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=max(a); [a1,ak1]=gsort(a); 
	if norm([am-a1(1,1),(ak(1)+(m)*(ak(2)-1))-ak1(1,1)])> %eps then pause,end
end

// test max(A) : A vector

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=max(a); [a1,ak1]=gsort(a); 
	if norm([am-a1(1,1),(ak-ak1(1,1))])> %eps then pause,end
end

// test mini 

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=max(-a); [ami,aki]=min(a);
	if norm([am+ami,ak-aki])> %eps then pause,end
end

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=max(-a); [ami,aki]=min(a);
	if norm([am+ami,ak-aki])> %eps then pause,end
end


// test max(A1,A....) compared with usual maxi + feval

m=10,n=20;
A1=rand(m,n);
A2=rand(m,n);
A3=rand(m,n);
A4=rand(m,n);
deff('[y]=f(i,j)','[y,k]=max([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);y=y+%i*k');
A=feval(1:m,1:n,f);
Am=real(A);Ak=imag(A);
[Am1,Ak1]=max(A1,A2,A3,A4);

if norm(Am1-Am)> %eps then pause,end
if norm(Ak1-Ak)> %eps then pause,end

// test max(list(A1,A2,..)) compared to max(A1,..)

[Al,Akl]=max(list(A1,A2,A3,A4));
if norm(Al-Am)> %eps then pause,end
if norm(Akl-Ak)> %eps then pause,end

// test max(A,'c') and max(A,'r') 
// max(A,'r') returns a row vector which contains max for each column

[Am,Akm]=max(A1,'r');
nc=size(A1,'c')
Am1=[];Ak1=[]
for i=1:nc,[am,ak]=max(A1(:,i)); Am1=[Am1,am];Ak1=[Ak1,ak];end
if norm(Am-Am1)> %eps then pause,end
if norm(Akm-Ak1)> %eps then pause,end


// max(A,'c');

[Am,Akm]=max(A1,'c');
nc=size(A1,'r')
Am1=[];Ak1=[]
for i=1:nc,[am,ak]=max(A1(i,:)); Am1=[Am1;am];Ak1=[Ak1;ak];end
if norm(Am-Am1)> %eps then pause,end
if norm(Akm-Ak1)> %eps then pause,end
