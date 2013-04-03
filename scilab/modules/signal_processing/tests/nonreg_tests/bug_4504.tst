// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4504 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4504
//
// <-- Short Description -->
// Function sskf does not act as the documentation says:
// Only xe = sskf(y,f,h,q,r,x0) works (not [xe pe] = sskf(y,f,h,q,r,x0))

rand("seed",5);
rand("normal");
q=[.03 0.01;.01 0.03];
u=rand(2,11);
f=[1.1 0.1;0 0.8];
g=(chol(q))';
m0=[10 10]';
p0=[2 0;0 2];
x0=m0+(chol(p0))'*rand(2,1);
x=ltitr(f,g,u,x0);
r=[2 0;0 2];
v=(chol(r))'*rand(2,11);
y=x+v;
h=eye(2,2);
assert_checktrue(execstr("[xe pe]=sskf(y,f,h,q,r,m0)","errcatch")==0);
