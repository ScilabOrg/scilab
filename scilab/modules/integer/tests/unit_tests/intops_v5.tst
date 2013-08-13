// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

warning("off");

funcprot(0);

//extractions
v=int8([1 2 3]);
assert_checkequal(v(2), int8(2));
assert_checkequal(v([1 3]), int8([1 3]));
assert_checkequal(v([3 3]), int8([3 3]));
assert_checkequal(v(:), int8([1 2 3]'));
assert_checkequal(v([]), []);

v=int32([1 2 3;4 5 6]);
assert_checkequal(v(2), int32(4));
assert_checkequal(v([1 3]), int32([1;2]));
assert_checkequal(v([3 3]), int32([2;2]));
assert_checkequal(v(:), int32([1;4;2;5;3;6]));
assert_checkequal(v([]), []);

assert_checkequal(v(2,1), int32(4));
assert_checkequal(v(1, [1 3]), int32([1 3]));
assert_checkequal(v(1, [3 3]), int32([3 3]));
assert_checkequal(v(1, :), int32([1 2 3]));
assert_checkequal(v(:, 2), int32([2;5]));
assert_checkequal(v([1 1], :), int32([1 2 3;1 2 3]));
assert_checkequal(v(:,[2 1]), int32([2 1;5 4]));

assert_checkequal(v(1,1:$), int32([1 2 3]));
assert_checkequal(v(1:$,2), int32([2;5]));
assert_checkequal(v([1 1], 1:$), int32([1 2 3;1 2 3]));
assert_checkequal(v(1:$, [2 1]), int32([2 1;5 4]));


//insertion
v=int8([1 2 3]);
v(2)=int8(3);
assert_checkequal(v, int8([1 3 3]));
v(4)=int8(-5);
assert_checkequal(v, int8([1 3 3 -5]));
v([1 4])=int8([2 5]);
assert_checkequal(v, int8([2 3 3 5])); 
v([1 1])=int8([2 5]);
assert_checkequal(v, int8([5 3 3 5]));
v([1 1 1 1 1 1])=int8([2 5 6 7 8 9]);
assert_checkequal(v, int8([9 3 3 5]));
v([4 2])=int8([8 5]);
assert_checkequal(v, int8([9 5 3 8]));

v=uint16([1 2 3]);
v(2)=uint16(3);
assert_checkequal(v, uint16([1 3 3]));
v(4)=uint16(-5);
assert_checkequal(v, uint16([1 3 3 -5]));
v([1 4])=uint16([2 5]);
assert_checkequal(v, uint16([2 3 3 5]));
v([1 1])=uint16([2 5]);
assert_checkequal(v, uint16([5 3 3 5]));
v([1 1 1 1 1 1])=uint16([2 5 6 7 8 9]);
assert_checkequal(v, uint16([9 3 3 5]));
v([4 2])=uint16([8 5]);
assert_checkequal(v, uint16([9 5 3 8]));

v=uint8([1 2 3]);
v(2,4)=uint8(10);
assert_checkequal(v, uint8([1 2 3 0;0 0 0 10]));
v([2 2],4)=uint8([10;20]);
assert_checkequal(v, uint8([1 2 3 0;0 0 0 20]));
v([2 1],4)=uint8([10;20]);
assert_checkequal(v, uint8([1 2 3 20;0 0 0 10]));
v([2 2 2],4)=uint8([10;20;30]);
assert_checkequal(v, uint8([1 2 3 20;0 0 0 30]));

v=int32([1 2 3]);
v(2,4)=int32(10);
assert_checkequal(v, int32([1 2 3 0;0 0 0 10]));
v([2 2],4)=int32([10;20]);
assert_checkequal(v, int32([1 2 3 0;0 0 0 20]));
v([2 1],4)=int32([10;20]);
assert_checkequal(v, int32([1 2 3 20;0 0 0 10]));
v([2 2 2],4)=int32([10;20;30]);
assert_checkequal(v, int32([1 2 3 20;0 0 0 30]));

v=int16([1 2 3;0 0 10]);
v(1,[1 1 1 1])=int16(1:4);
assert_checkequal(v, int16([4 2 3;0 0 10]));
v([2 2],[1 1 1 1])=int16([1:4;2:5]);
assert_checkequal(v, int16([4 2 3;5 0 10]));
v(3,4)=int16(7);
assert_checkequal(v, int16([4 2 3 0;5 0 10 0;0 0 0 7]));

v=uint32([1 2 3;0 0 10]);
v(1,[1 1 1 1])=uint32(1:4);
assert_checkequal(v, uint32([4 2 3;0 0 10]));
v([2 2],[1 1 1 1])=uint32([1:4;2:5]);
assert_checkequal(v, uint32([4 2 3;5 0 10]));
v(3,4)=uint32(7);
assert_checkequal(v, uint32([4 2 3 0;5 0 10 0;0 0 0 7]));

v=[];
v(5)=int8(1);
assert_checkequal(v, int8([0;0;0;0;1]));
v(2)=int8(4);
assert_checkequal(v, int8([0;4;0;0;1]));

v=uint16([1 2 3;4 5 6]);
v(:,1)=uint16([4;1]);
assert_checkequal(v, uint16([4 2 3;1 5 6]));
v(:,[1 1])=uint16([4 5;1 2]);
assert_checkequal(v, uint16([5 2 3; 2 5 6]));
v(:,[3 1])=uint16([4 5;1 2]);
assert_checkequal(v, uint16([5 2 4;2 5 1]));
v(:,[3 3 3 3])=uint16([4 5 6 7;1 2 3 4]);
assert_checkequal(v, uint16([5 2 7;2 5 4]));

v=int8([1 2 3;4 5 6]);
v(1,:)=int8([2 3 4]);
assert_checkequal(v, int8([2 3 4;4 5 6]));
v([1 1],:)=int8([2 3 4;4 5 6]);
assert_checkequal(v, int8([4 5 6;4 5 6]));
v([2 2 2 2],:)=int8([2 3 4;4 5 6;7 8 9;10 11 12]);
assert_checkequal(v, int8([4 5 6;10 11 12]));

v=uint8([1 2 3]);
v(2)=[];
assert_checkequal(v, uint8([1 3]));
v=uint8([1 2 3]);
v([3 2])=[];
assert_checkequal(v, uint8(1));

v=int8([1 2 3]');
v(2)=[];
assert_checkequal(v, int8([1 3]'));
v=int8([1 2 3]');
v([3 2])=[];
assert_checkequal(v, int8(1));

v=int32([1 2 3;4 5 6]);
v(1,:)=[];
assert_checkequal(v, int32([4 5 6]));
v=int32([1 2 3;4 5 6]);
v(:,[3 2])=[];
assert_checkequal(v, int32([1;4]));

//hidden empty index in insertion
a=int32([1 2;3 4]);a_ref=a;
a(1,[%f %f])=[];
assert_checkequal(a, a_ref);
a([%f %f],1)=[];
assert_checkequal(a, a_ref);
a([%f %f],[%f %f])=[];
assert_checkequal(a, a_ref);

a=int8([1 2;3 4]);a_ref=a;
a(1,[%f %f])=[];
assert_checkequal(a, a_ref);
a([%f %f],1)=[];
assert_checkequal(a, a_ref);
a([%f %f],[%f %f])=[];
assert_checkequal(a, a_ref);


//concatenations
x1=int8(1);x2=int8(7);
assert_checkequal([x1 x2], int8([1 7]));
assert_checkequal([x1;x2], int8([1;7]));
assert_checkequal([x1 []], int8([1]));
assert_checkequal([x1;[]], int8([1]));
assert_checkequal([[] x2], int8([7]));
assert_checkequal([[];x2], int8([7]));

[x1,x2]=([x1 x2 x1 x2],[x2 x2 x1 x1]);
assert_checkequal([x1 x2], int8([1 7 1 7 7 7 1 1]));
assert_checkequal([x1 []], int8([1 7 1 7]));
assert_checkequal([x1;[]], int8([1 7 1 7]));
assert_checkequal([[] x2], int8([7 7 1 1]));
assert_checkequal([[; x2]], int8([7 7 1 1]));

n=300;x1=uint16(1:n);
assert_checkequal([x1 x1], uint16([1:n 1:n]));
assert_checkequal([x1;x1], uint16([1:n; 1:n]));

//additions soustractions
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1+x2, int8(X1+X2));
assert_checkequal(x1-x2, int8(X1-X2));

X1=1:10;x1=int8(X1);
assert_checkequal(x1+x2, int8(X1+X2));
assert_checkequal(x1-x2, int8(X1-X2));

X2=2:11;x2=int8(X2);
assert_checkequal(x1+x2, int8(X1+X2));
assert_checkequal(x1-x2, int8(X1-X2));
assert_checkequal(-x1, int8(-(1:10)));

X2=[];x2=[];
assert_checkequal(x1+x2, int8(X1+X2));
assert_checkequal(x1-x2, int8(X1-X2));
assert_checkequal(x2+x1, int8(X2+X1));
assert_checkequal(x2-x1, int8(X2-X1));

//multiplication
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1*x2, int8(X1*X2));
assert_checkequal(x1.*x2, int8(X1.*X2));

X1=1:10;x1=int8(X1);
assert_checkequal(x1*x2, int8(X1*X2));
assert_checkequal(x1.*x2, int8(X1.*X2));
assert_checkequal(x2*x1, int8(X2*X1));
assert_checkequal(x2.*x1, int8(X2.*X1));

X2=-(1:10);x2=int8(X2);
assert_checkequal(x1.*x2, int8(X1.*X2));
assert_checkequal(x1'*x2, int8(X1'*X2));
assert_checkequal(x1*x2', int8(X1*X2'));

// comparaisons
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1>x2, X1>X2);
assert_checkequal(x1>=x2, X1>=X2);
assert_checkequal(x1<x2, X1<X2);
assert_checkequal(x1<=x2, X1<=X2);
assert_checkequal(x1<>x2, X1<>X2);

X1=1:10;x1=int8(X1);
assert_checkequal(x1>x2, X1>X2);
assert_checkequal(x1>=x2, X1>=X2);
assert_checkequal(x1<x2, X1<X2);
assert_checkequal(x1<=x2, X1<=X2);
assert_checkequal(x1<>x2, X1<>X2);

X2=2:11;x2=int8(X2);
assert_checkequal(x1>x2, X1>X2);
assert_checkequal(x1>=x2, X1>=X2);
assert_checkequal(x1<x2, X1<X2);
assert_checkequal(x1<=x2, X1<=X2);
assert_checkequal(x1<>x2, X1<>X2);

X1=1;x1=int8(X1);
assert_checkequal(x1>x2, X1>X2);
assert_checkequal(x1>=x2, X1>=X2);
assert_checkequal(x1<x2, X1<X2);
assert_checkequal(x1<=x2, X1<=X2);
assert_checkequal(x1<>x2, X1<>X2);

//division
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1/x2, int8(0));
assert_checkequal(x2/x1, int8(7));
assert_checkequal(x1\x2, int8(7));
assert_checkequal(x2\x1, int8(0));
assert_checkequal(x1./x2, int8(0));
assert_checkequal(x2./x1, int8(7));
assert_checkequal(x1.\x2, int8(7));
assert_checkequal(x2.\x1, int8(0));

X1=1:10;x1=int8(X1);
assert_checkequal(x1/x2, int8([0 0 0 0 0 0 1 1 1 1]));
assert_checkequal(x1./x2, int8([0 0 0 0 0 0 1 1 1 1]));
assert_checkequal(x2./x1, int8([7 3 2 1 1 1 1 0 0 0]));
assert_checkequal(x1.\x2, int8([7 3 2 1 1 1 1 0 0 0]));
assert_checkequal(x2\x1, int8([0 0 0 0 0 0 1 1 1 1]));
assert_checkequal(x2.\x1, int8([0 0 0 0 0 0 1 1 1 1]));
//if or(x2/x1<>int8([7 3 2 1 1 1 1 0 0 0])) then pause,end
//if or(x1\x2<>int8([0 0 0 0 0 0 1 1 1 1])) then pause,end

X2=-(1:10);x2=int8(X2);
assert_checkequal(x1./x2, int8(-ones(1,10)));
assert_checkequal(x1.\x2, int8(-ones(1,10)));

// puissance
X1=2;X2=3;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1^x2, int8(X1^X2));
assert_checkequal(x1.^x2, int8(X1.^X2));

X1=1:5;x1=int8(X1);
assert_checkequal(x1.^x2, int8(X1.^X2));

X1=2;x1=int8(X1);X2=(1:3);x2=int8(X2);
assert_checkequal(x1.^x2, int8(X1.^X2));

//kronecker
X1=2;X2=3;x1=int8(X1);x2=int8(X2);
assert_checkequal(x1.*.x2, int8(X1.*.X2));

X1=1:5;x1=int8(X1);
assert_checkequal(x1.*.x2, int8(X1.*.X2));

X1=2;x1=int8(X1);X2=(1:3);x2=int8(X2);
assert_checkequal(x1.*.x2, int8(X1.*.X2));

X1=(1:5)';x1=int8(X1);
assert_checkequal(x1.*.x2, int8(X1.*.X2));

//sum
X=round(10*rand(1,30));
deff('y=INT(x)','y=int16(x)')
x=INT(X);
assert_checkequal(sum(x), INT(sum(X)));
assert_checkequal(sum(x'), INT(sum(X')));
assert_checkequal(cumsum(x), INT(cumsum(X)));
assert_checkequal(cumsum(x'), INT(cumsum(X')));

deff('y=INT(x)','y=uint16(x)')
x=INT(X);
assert_checkequal(sum(x), INT(sum(X)));
assert_checkequal(sum(x'), INT(sum(X')));
assert_checkequal(cumsum(x), INT(cumsum(X)));
assert_checkequal(cumsum(x'), INT(cumsum(X')));

deff('y=INT(x)','y=uint8(x)')
x=INT(X);
assert_checkequal(sum(x), INT(sum(X)));
assert_checkequal(sum(x'), INT(sum(X')));
assert_checkequal(cumsum(x), INT(cumsum(X)));
assert_checkequal(cumsum(x'), INT(cumsum(X')));

deff('y=INT(x)','y=int8(x)')
x=INT(X);
assert_checkequal(sum(x), INT(sum(X)));
assert_checkequal(sum(x'), INT(sum(X')));
assert_checkequal(cumsum(x), INT(cumsum(X)));
assert_checkequal(cumsum(x'), INT(cumsum(X')));

X=round(10*rand(20,30));
x=INT(X);
assert_checkequal(sum(x,1), INT(sum(X,1)));
assert_checkequal(sum(x',1), INT(sum(X',1)));
assert_checkequal(cumsum(x,1), INT(cumsum(X,1)));
assert_checkequal(cumsum(x',1), INT(cumsum(X',1)));

deff('y=INT(x)','y=uint16(x)')
x=INT(X);
assert_checkequal(sum(x,1), INT(sum(X,1)));
assert_checkequal(sum(x',1), INT(sum(X',1)));
assert_checkequal(cumsum(x,1), INT(cumsum(X,1)));
assert_checkequal(cumsum(x',1), INT(cumsum(X',1)));

deff('y=INT(x)','y=uint8(x)')
x=INT(X);
assert_checkequal(sum(x,1), INT(sum(X,1)));
assert_checkequal(sum(x',1), INT(sum(X',1)));
assert_checkequal(cumsum(x,1), INT(cumsum(X,1)));
assert_checkequal(cumsum(x',1), INT(cumsum(X',1)));

deff('y=INT(x)','y=int8(x)')
x=INT(X);
assert_checkequal(sum(x,1), INT(sum(X,1)));
assert_checkequal(sum(x',1), INT(sum(X',1)));
assert_checkequal(cumsum(x,1), INT(cumsum(X,1)));
assert_checkequal(cumsum(x',1), INT(cumsum(X',1)));

x=INT(X);
assert_checkequal(sum(x,'c'), INT(sum(X,'c')));
assert_checkequal(sum(x','c'), INT(sum(X','c')));
assert_checkequal(cumsum(x,'c'), INT(cumsum(X,'c')));
assert_checkequal(cumsum(x','c'), INT(cumsum(X','c')));

deff('y=INT(x)','y=uint16(x)')
x=INT(X);
assert_checkequal(sum(x,'c'), INT(sum(X,'c')));
assert_checkequal(sum(x','c'), INT(sum(X','c')));
assert_checkequal(cumsum(x,'c'), INT(cumsum(X,'c')));
assert_checkequal(cumsum(x','c'), INT(cumsum(X','c')));

deff('y=INT(x)','y=uint8(x)')
x=INT(X);
assert_checkequal(sum(x,'c'), INT(sum(X,'c')));
assert_checkequal(sum(x','c'), INT(sum(X','c')));
assert_checkequal(cumsum(x,'c'), INT(cumsum(X,'c')));
assert_checkequal(cumsum(x','c'), INT(cumsum(X','c')));

deff('y=INT(x)','y=int8(x)')
x=INT(X);
assert_checkequal(sum(x,'c'), INT(sum(X,'c')));
assert_checkequal(sum(x','c'), INT(sum(X','c')));
assert_checkequal(cumsum(x,'c'), INT(cumsum(X,'c')));
assert_checkequal(cumsum(x','c'), INT(cumsum(X','c')));

//diag triu tril
deff('y=INT(x)','y=int16(x)')

X=round(10*rand(1,30));
x=INT(X);
assert_checktrue(diag(x) == diag(X));
assert_checktrue(diag(x, 2) == diag(X, 2));
assert_checktrue(diag(x, -2) == diag(X, -2));

assert_checktrue(triu(x) == triu(X));
assert_checktrue(triu(x, 2) == triu(X, 2));
assert_checktrue(triu(x, -2) == triu(X, -2));

assert_checktrue(tril(x) == tril(X));
assert_checktrue(tril(x, 2) == tril(X, 2));
assert_checktrue(tril(x, -2) == tril(X, -2));

X=round(10*rand(2,30));
x=INT(X);
assert_checktrue(diag(x) == diag(X));
assert_checktrue(diag(x, 2) == diag(X, 2));
assert_checktrue(diag(x, -2) == diag(X, -2));

assert_checktrue(triu(x) == triu(X));
assert_checktrue(triu(x, 2) == triu(X, 2));
assert_checktrue(triu(x, -2) == triu(X, -2));

assert_checktrue(tril(x) == tril(X));
assert_checktrue(tril(x, 2) == tril(X, 2));
assert_checktrue(tril(x, -2) == tril(X, -2));

deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30));
x=INT(X);

assert_checktrue(diag(x) == diag(X));
assert_checktrue(diag(x, 2) == diag(X, 2));
assert_checktrue(diag(x, -2) == diag(X, -2));

assert_checktrue(triu(x) == triu(X));
assert_checktrue(triu(x, 2) == triu(X, 2));
assert_checktrue(triu(x, -2) == triu(X, -2));

assert_checktrue(tril(x) == tril(X));
assert_checktrue(tril(x, 2) == tril(X, 2));
assert_checktrue(tril(x, -2) == tril(X, -2));

X=round(10*rand(2,30));
x=INT(X);

assert_checktrue(diag(x) == diag(X));
assert_checktrue(diag(x, 2) == diag(X, 2));
assert_checktrue(diag(x, -2) == diag(X, -2));

assert_checktrue(triu(x) == triu(X));
assert_checktrue(triu(x, 2) == triu(X, 2));
assert_checktrue(triu(x, -2) == triu(X, -2));

assert_checktrue(tril(x) == tril(X));
assert_checktrue(tril(x, 2) == tril(X, 2));
assert_checktrue(tril(x, -2) == tril(X, -2));

// save load
deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30));
x=INT(X);

save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');
assert_checkequal(x, INT(X));

x=list(1,x,2);save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');
assert_checkequal(x, list(1, INT(X), 2));

deff('y=INT(x)','y=int32(x)')
X=round(10*rand(1,30));
x=INT(X);

save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');
assert_checkequal(x, INT(X));

x=list(1,x,2);save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');
assert_checkequal(x, list(1, INT(X), 2));

//abs
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);
assert_checkequal(abs(x), INT(abs(X)));

deff('y=INT(x)','y=int16(x)')
X=round(10*rand(1,30))-5;
x=INT(X);
assert_checkequal(abs(x), INT(abs(X)));

//matrix
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);
assert_checkequal(matrix(x,10,-1), INT(matrix(X,10,-1)));
assert_checkequal(matrix(x,10,3), INT(matrix(X,10,3)));
assert_checkequal(matrix(x,-1,3), INT(matrix(X,-1,3)));

deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);
assert_checkequal(matrix(x,10,-1), INT(matrix(X,10,-1)));
assert_checkequal(matrix(x,10,3), INT(matrix(X,10,3)));
assert_checkequal(matrix(x,-1,3), INT(matrix(X,-1,3)));

//max min
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(2,30))-5;
Y=ones(2,30);
x=INT(X);
y=INT(Y);
assert_checkequal(max(x), INT(max(X)));
assert_checkequal(max(x, 'r'), INT(max(X, 'r')));
assert_checkequal(max(x, 'c'), INT(max(X, 'c')));
assert_checkequal(max(x, INT(2)), INT(max(X, 2)));
assert_checkequal(max(x, y), INT(max(X, Y)));

assert_checkequal(min(x), INT(min(X)));
assert_checkequal(min(x, 'r'), INT(min(X, 'r')));
assert_checkequal(min(x, 'c'), INT(min(X, 'c')));
assert_checkequal(min(x, INT(2)), INT(min(X, 2)));
assert_checkequal(min(x, y), INT(min(X, Y)));

a=[0 3 7];
b=[1 2 7];
A=[0 3 7 5;
   1 2 0 2
   8 9 3 1];
types=['double','int32','uint32','int16','uint16','int8','uint8'];

// checking max(a,b)
[mx,kx]=max(a,b);
for t1=types
  for t2=types
    execstr('at='+t1+'(a); bt='+t2+'(b);')
    [mt,kt]=max(at,bt);
    if or(kt<>kx)|or(mx<>double(mt)) then 
      disp('max('+t1+','+t2+')'),pause
    end
  end	
end

// checking min(a,b)
[mn,kn]=min(a,b);
for t1=types
  for t2=types
    execstr('at='+t1+'(a); bt='+t2+'(b);')
    [mt,kt]=min(at,bt);
    if or(kt<>kn)|or(mn<>double(mt)) then 
      disp('min('+t1+','+t2+')'),pause
    end
  end	
end

// checking max(A)
[mx,kx]=max(A)
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At);
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+')'),pause
  end
end

// checking min(A)
[mx,kx]=min(A)
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At);
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+')'),pause
  end
end


// checking max(A,'r')
[mx,kx]=max(A,'r')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At,'r');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+',''r'')'),pause
  end
end

// checking min(A,'r')
[mx,kx]=min(A,'r')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At,'r');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+',''r'')'),pause
  end
end

// checking max(A,'c')
[mx,kx]=max(A,'c')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At,'c');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+',''c'')'),pause
  end
end

// checking min(A,'c')
[mx,kx]=min(A,'c')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At,'c');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+',''c'')'),pause
  end
end


//int32 int8 ... iconvert
