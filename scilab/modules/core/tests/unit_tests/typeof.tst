// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// unit test for typeof()

// constant
assert_checkequal(typeof(1),"constant");
assert_checkequal(typeof([1,1]),"constant");
assert_checkequal(typeof([1;1]),"constant");
assert_checkequal(typeof([1,1;1,1]),"constant");
assert_checkequal(typeof([1+%i,1+%i;1,1]),"constant");

// polynomial
assert_checkequal(typeof(poly(0,"s")),"polynomial");

// boolean
assert_checkequal(typeof(%t),"boolean");
assert_checkequal(typeof([%t,%t;%f,%f]),"boolean");
assert_checkequal(typeof("s"=="s"),"boolean");

// sparse
assert_checkequal(typeof(sparse([0,1,0,1:1,0,1,0])),"sparse");

// boolean sparse
assert_checkequal(typeof(sparse([1,1;2,2;3,3],[%t,%t,%t])),"boolean sparse");

// Matlab sparse
X=sparse(rand(2,2)); Y=mtlb_sparse(X);
assert_checkequal(typeof(Y),"Matlab sparse");

// int
assert_checkequal(typeof(int8(1:4)),"int8");
assert_checkequal(typeof(uint8(1:4)),"uint8");
assert_checkequal(typeof(int16(1:4)),"int16");
assert_checkequal(typeof(uint16(1:4)),"uint16");
assert_checkequal(typeof(int32(1:4)),"int32");
assert_checkequal(typeof(uint32(1:4)),"uint32");
format("e");
assert_checkequal(typeof(uint8(1:4)),"uint8"); // bug 8687
format("v");

// handle
plot();
f=gcf();
assert_checkequal(typeof(f),"handle");
a=gca();
assert_checkequal(typeof(a),"handle");
e=gce();
assert_checkequal(typeof(e),"handle");
assert_checkequal(typeof(a.children(1)),"handle");
close

// string
assert_checkequal(typeof("s"),"string");
assert_checkequal(typeof("string"),"string");
assert_checkequal(typeof(["s","s"]),"string");

// function
deff("[x]=myplus(y,z)","x=y+z");
assert_checkequal(typeof(myplus),"function");

// library
assert_checkequal(typeof(elementary_functionslib),"library");
assert_checkequal(typeof(corelib),"library");

// pointer
a=rand(5,5);b=rand(5,1);A=sparse(a);
[h,rk]=lufact(A);
x=lusolve(h,b);a*x-b;
ludel(h);
assert_checkequal(typeof(h),"pointer");

// size implicit
assert_checkequal(typeof(1:1:$), "size implicit");

// list
assert_checkequal(typeof(list(1,2)),"list");
t = tlist(["listtype","field1","field2"], [], []);
assert_checkequal(typeof(t),t(1)(1));
M = mlist(["V","name","value"],["a","b","c"],[1 2 3]);
M1=getfield(1,M);
assert_checkequal(typeof(M),M1(1));

// rational, state-space, uitree
assert_checkequal(typeof(1/poly(0,"s")),"rational");
A=[0,1;0,0];B=[1;1];C=[1,1];
S1=syslin("c",A,B,C);
assert_checkequal(typeof(S1),"state-space");
S2=syslin("d",A,B,C);
assert_checkequal(typeof(S2),"state-space");
S3=syslin(1,A,B,C);
assert_checkequal(typeof(S3),"state-space");
assert_checkequal(typeof(tf2ss(1/poly(0,"s"))),"state-space");
l1 = uiCreateNode("CONST_m","default","");
assert_checkequal(typeof(l1),"uitree");
tree = uiCreateTree(l1);
assert_checkequal(typeof(tree),"uitree");

// hypermat
assert_checkequal(typeof(hypermat([2 3 2 2],1:24)),"hypermat");
A=[1,2,3;4,5,6];
A(:,:,2)=[1,2,3;4,5,6];
assert_checkequal(typeof(A),"hypermat");

// fptr
assert_checkequal(typeof(abs),"fptr");

// cell
assert_checkequal(typeof(cell(3)),"ce");

//structure
assert_checkequal(typeof(struct("jour",25,"mois","DEC","annee",2006)),"st");
