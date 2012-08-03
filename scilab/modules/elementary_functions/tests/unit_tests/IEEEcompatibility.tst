// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2009 - INRIA - Michael Baudin, Antoine Elias
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- CLI SHELL MODE -->
// <-- Bugzilla URL -->
//


// Bibliography :
// "IEEE Standard for Floating-Point Arithmetic"
// IEEE Std 754-2008
// 29 August 2008
// (Revision of IEEE Std 754-1985)

// Check that properties of double precision floating point numbers
// are the one which are expected.
// Strict equality is required here.
radix = number_properties("radix");
assert_checkequal ( radix , 2 );
p = number_properties("digits");
assert_checkequal ( p , 53 );
eps = number_properties("eps");
assert_checkequal ( eps , 1.11022302462515650e-016 );
assert_checkequal ( %eps , 2.22044604925031310e-016 );
huge = number_properties("huge");
assert_checkequal ( huge , 1.797693134862315708e+308 );
tiny = number_properties("tiny");
assert_checkequal ( tiny , 2.22507385850720140e-308 );
denorm = number_properties("denorm");
assert_checkequal ( denorm , %t );
tiniest = number_properties("tiniest");
assert_checkequal ( tiniest , 4.94065645841246540e-324 );
minexp = number_properties("minexp");
assert_checkequal ( minexp , -1021 );
maxexp = number_properties("maxexp");
assert_checkequal ( maxexp , 1024 );

// Check that elementary operators +,-,*,/,sqrt and modulo
// satisfy basic IEEE requirements.

expectedadd = [
-%inf 	-%inf 	-%inf 	-%inf 	-%inf 	%nan 	%nan
-%inf 	-2 	-1 	-1 	0 	%inf 	%nan
-%inf 	-1 	-0 	0 	1 	%inf 	%nan
-%inf 	-1 	0 	0 	1 	%inf 	%nan
-%inf 	0 	1 	1 	2 	%inf 	%nan
%nan 	%inf 	%inf 	%inf 	%inf 	%inf 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expectedminus = [
%nan 	-%inf 	-%inf 	-%inf 	-%inf 	-%inf 	%nan
%inf 	0 	-1 	-1 	-2 	-%inf 	%nan
%inf 	1 	0 	-0 	-1 	-%inf 	%nan
%inf 	1 	0 	0 	-1 	-%inf 	%nan
%inf 	2 	1 	1 	0 	-%inf 	%nan
%inf 	%inf 	%inf 	%inf 	%inf 	%nan 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expectedmult = [
%inf 	%inf 	%nan 	%nan 	-%inf 	-%inf 	%nan
%inf 	1 	0 	-0 	-1 	-%inf 	%nan
%nan 	0 	0 	-0 	-0 	%nan 	%nan
%nan 	-0 	-0 	0 	0 	%nan 	%nan
-%inf 	-1 	-0 	0 	1 	%inf 	%nan
-%inf 	-%inf 	%nan 	%nan 	%inf 	%inf 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expecteddiv = [
%nan 	%inf 	%inf 	-%inf 	-%inf 	%nan 	%nan
0 	1 	%inf 	-%inf 	-1 	-0 	%nan
0 	0 	%nan 	%nan 	-0 	-0 	%nan
-0 	-0 	%nan 	%nan 	0 	0 	%nan
-0 	-1 	-%inf 	%inf 	1 	0 	%nan
%nan 	-%inf 	-%inf 	%inf 	%inf 	%nan 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

// See http://bugzilla.scilab.org/show_bug.cgi?id=2409
// We must use imult(%inf) to get the mathematical number i * inf
// since %i * %inf produces imult(%inf), as expected by the 
// intermediate multiplication 0*%inf = %nan.
expectedsqrt = [
imult(%inf)
%i
0
0
1.
%inf
%nan];

expectedmodulo = [
%nan  %nan  %nan  %nan  %nan  %nan  %nan
%nan     0. %nan  %nan     0. %nan  %nan
%nan     0. %nan  %nan     0. %nan  %nan
%nan     0. %nan  %nan     0. %nan  %nan
%nan     0. %nan  %nan     0. %nan  %nan
%nan  %nan   %nan  %nan  %nan  %nan  %nan
%nan  %nan   %nan  %nan  %nan  %nan  %nan];

left=[-%inf,-1,-0,+0,1,%inf,%nan];
right=left;
n=size(left,2);

computedadd = zeros(n,n);
computedminus = zeros(n,n);
computedmult = zeros(n,n);
computeddiv = zeros(n,n);
computedsqrt = zeros(n);
computedsqrt = zeros(n);
computedmodulo = zeros(n,n);
ieee(2)
for i=1:n
    l=left(i);
    computedsqrt (i) = sqrt(l);
    for j=1:n
        r=right(j);
        computedadd (i,j)=l+r;
        computedminus (i,j)=l-r;
        computedmult (i,j)=l*r;
        computeddiv (i,j)=l/r;
        computedmodulo (i,j) = modulo(l , r);
    end
end

assert_checkequal ( computedadd    , expectedadd );
assert_checkequal ( computedminus  , expectedminus );
assert_checkequal ( computedmult   , expectedmult );
assert_checkequal ( computeddiv    , expecteddiv );
assert_checkequal ( computedsqrt   , expectedsqrt );
assert_checkequal ( computedmodulo , expectedmodulo );

ieee(0)

