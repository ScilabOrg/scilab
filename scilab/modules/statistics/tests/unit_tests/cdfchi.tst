// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfchi() function
// =============================================================================

prec = 1.e-5;

x  = [0.01,0.025,0.05,0.1,0.9,0.95];
deff('[y]=chi1(x)','y=exp(-x/2)/sqrt(2*%pi*x)');
y1 = [];

for xx=x
	y1=[y1,intg(0,xx,chi1)];
end

y2 = cdfchi("PQ",x,ones(x));
if norm(y1-y2) > prec then pause,end 

df       = [1,2,3,4,5,6];
[P,Q]    = cdfchi("PQ",x,df);
[x1]     = cdfchi("X",df,P,Q);
[df1]    = cdfchi("Df",P,Q,x);

if norm(x1-x)   > prec then pause,end
if norm(df1-df) > prec then pause,end

// Df should be an integer matrix
df      = df - 0.5;
refMsg2 = msprintf(_("%s: Wrong value for input argument #%d: A matrix of integer value expected.\n"), "cdfchi", 2);
refMsg3 = msprintf(_("%s: Wrong value for input argument #%d: A matrix of integer value expected.\n"), "cdfchi", 3);
assert_checkerror("cdfchi(""PQ"",x,df);",refMsg3);
assert_checkerror("cdfchi(""X"",df,P,Q);",refMsg2);
