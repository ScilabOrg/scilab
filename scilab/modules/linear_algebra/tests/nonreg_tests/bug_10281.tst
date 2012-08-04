// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10281 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10281
//
// <-- Short Description -->
// lyap segfaults in some cases

s=poly(0,'s');
A=[-1 10;0 1];B=[-2;0];C=[-2 3];D=[-2];
sis57=syslin('c',A,B,C,D);
ss2tf(sis57);

gs=C*inv((s*eye(2,2)-A))*B+D;

C=[1 1];
A=[0 1;-0.5 -1];
X=lyap(A,C,'c');
assert_checkalmostequal(X,[-1.5,-1]);
