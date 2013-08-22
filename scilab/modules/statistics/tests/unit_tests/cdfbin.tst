// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfbin() function
//
// CN^k pr^k(1-pr)^(Xb-k)
//
// =============================================================================

prec = 1.e-5;


XN = 10;
PR = 0.34;

deff("k=fact(n)","if n<=1;k=1;else k=n*fact(n-1);end");
deff("cnk=CNK(N,k)","cnk=fact(N)/(fact(k)*fact(N-k))");

pr  = [];
S   = 0:XN;

for k=S
    pr=[pr,CNK(XN,k)*(PR)^k*(1-PR)^(XN-k)];
end

Sth      = cumsum(pr);
[Sth1,q] = cdfbin("PQ",S,XN*ones(S),PR*ones(S),(1-PR)*ones(S));
if norm(Sth-Sth1) > prec then pause,end

XN          = 10;
S           = 0:XN;
PR          = rand(1,XN+1,"u");
OMPR        = 1-PR;
XN1         = XN*ones(1,XN+1);
[P,Q]       = cdfbin("PQ",S,XN1,PR,OMPR);
[S1]        = cdfbin("S",XN1,PR,OMPR,P,Q);
[XN2]       = cdfbin("Xn",PR,OMPR,P,Q,S);
[PR1,OMPR1] = cdfbin("PrOmpr",P,Q,S,XN1);

if norm(S1-S)                  > prec  then pause,end
if norm(XN1(1:$-1)-XN2(1:$-1)) > 10*prec  then pause,end

// not good when pr is near 1 or zero
if norm(PR1(1:$-1)-PR(1:$-1)) > 0.1  then pause,end
