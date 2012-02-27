// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

Ntest=1;

// test min(A) : A matrix
// max is compared with sort

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=min(a); [a1,ak1]=gsort(a, 'g', 'i');
	assert_checkequal(am, a1(1));
	assert_checkequal(ak(1) + m * (ak(2) - 1), ak1(1));
end

// test min(A) : A vector

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=min(a); [a1,ak1]=gsort(a, 'g', 'i');
    assert_checkequal(am, a1(1));
	assert_checkequal(ak, ak1(1));
end

// test max and min

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=max(-a); [ami,aki]=min(a);
	assert_checkequal(-1 * am, ami);
	assert_checkequal(ak, aki);
end

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=max(-a); [ami,aki]=min(a);
	assert_checkequal(-1 * am, ami);
	assert_checkequal(ak, aki);
end

// test min(A1,A....) compared with usual min + feval

m=10;n=20;
A1=rand(m,n);
A2=rand(m,n);
A3=rand(m,n);
A4=rand(m,n);

function [y] = f(i,j)
    [y,k] = min([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);
    y = y + %i * k;
end

A=feval(1:m,1:n,f);
Am=real(A);Ak=imag(A);
[Am1,Ak1]=min(A1,A2,A3,A4);

assert_checkequal(Am1,Am);
assert_checkequal(Ak1,Ak);

// test min(list(A1,A2,..)) compared to min(A1,..)

[Al,Akl]=min(list(A1,A2,A3,A4));
assert_checkequal(Al,Am);
assert_checkequal(Akl,Ak);

// test min(A,'c') and min(A,'r')
// min(A,'r') returns a row vector which contains min for each column

[Am,Akm]=min(A1,'r');
nc=size(A1,'c');
Am1=[];Ak1=[];
for i=1:nc,[am,ak]=min(A1(:,i)); Am1=[Am1,am];Ak1=[Ak1,ak];end
assert_checkequal(Am1,Am);
assert_checkequal(Akm,Ak1);

// min(A,'c');

[Am,Akm]=min(A1,'c');
nr=size(A1,'r');
Am1=[];Ak1=[];
for i=1:nr,[am,ak]=min(A1(i,:)); Am1=[Am1;am];Ak1=[Ak1;ak];end
assert_checkequal(Am1,Am);
assert_checkequal(Akm,Ak1);

// min(A, scalar)
[am ak] = min(1:10, 5);
assert_checkequal(am(1:5), [1:5]);
assert_checkequal(am(6:10), 5 * ones(1, 5));
assert_checkequal(ak(1:5), ones(1, 5));
assert_checkequal(ak(6:10), 2 * ones(1, 5));

// ---------- Sparse ----------

// test min(A) : A matrix
// max is compared with sort

for i=1:Ntest,
	m=100;n=200;a=sparse(rand(m,n));
	[am,ak]=min(a); [a1,ak1]=gsort(a(:), 'g', 'i');
	assert_checkequal(am, a1(1));
	assert_checkequal(ak(1) + m * (ak(2) - 1), ak1(1));
end

// test min(A) : A vector

for i=1:Ntest,
	m=100;a=sparse(rand(1,m));
	[am,ak]=min(a); [a1,ak1]=gsort(a(:), 'g', 'i');
    assert_checkequal(am, a1(1));
	assert_checkequal(ak, ak1(1));
end

// test max and min

for i=1:Ntest,
	m=100;n=200;a=spars(rand(m,n));
	[am,ak]=max(-a); [ami,aki]=min(a);
	assert_checkequal(-1 * am, ami);
	assert_checkequal(ak, aki);
end

for i=1:Ntest,
	m=100;a=sparse(rand(1,m));
	[am,ak]=max(-a); [ami,aki]=min(a);
	assert_checkequal(-1 * am, ami);
	assert_checkequal(ak, aki);
end

// test min(A1,A....) compared with usual min + feval

m=10;n=20;
A1=sparse(rand(m,n));
A2=sparse(rand(m,n));
A3=sparse(rand(m,n));
A4=sparse(rand(m,n));

function [y] = f(i,j)
    [y,k] = min([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);
    y = y + %i * k;
end

A=feval(1:m,1:n,f);
Am=real(A);Ak=imag(A);
[Am1,Ak1]=min(A1,A2,A3,A4);

assert_checkequal(Am1,Am);
assert_checkequal(Ak1,Ak);

// test min(list(A1,A2,..)) compared to min(A1,..)

[Al,Akl]=min(list(A1,A2,A3,A4));
assert_checkequal(Al,Am);
assert_checkequal(Akl,Ak);

// test min(A,'c') and min(A,'r')
// min(A,'r') returns a row vector which contains min for each column

[Am,Akm]=min(A1,'r');
nc=size(A1,'c');
Am1=[];Ak1=[];
for i=1:nc,[am,ak]=min(A1(:,i)); Am1=[Am1,am];Ak1=[Ak1,ak];end
assert_checkequal(Am1,Am);
assert_checkequal(Akm,Ak1);

// min(A,'c');

[Am,Akm]=min(A1,'c');
nr=size(A1,'r');
Am1=[];Ak1=[];
for i=1:nr,[am,ak]=min(A1(i,:)); Am1=[Am1;am];Ak1=[Ak1;ak];end
assert_checkequal(Am1,Am);
assert_checkequal(Akm,Ak1);
