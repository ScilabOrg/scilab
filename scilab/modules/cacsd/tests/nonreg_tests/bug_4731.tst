// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 4731 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4731
//
// <-- Short Description -->
// lqr fails if dt argument of syslin is a number

A=[0.5 0.3;0.3 0.9];B=[0.9; 0.3];
Q=diag([2,5]);R=2;
Big=sysdiag(Q,R);
[w,wp]=fullrf(Big);C1=wp(:,1:2);D12=wp(:,3:$);
P=syslin(1,A,B,C1,D12);
[K,X]=lqr(P);
S=spec(A+B*K);
R=norm(A'*X*A-(A'*X*B)*pinv(B'*X*B+R)*(B'*X*A)+Q-X,1);
assert_checkalmostequal(real(S),[0.24977769272823835; 0.47346570058644721]);
assert_checkequal(imag(S),[0;0]);
assert_checktrue(R<100*%eps);
