// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte Hecquet
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//Generate the process
t1=0:0.1:100;
y1=sin(2*%pi*t1)+sin(2*%pi*2*t1);

//Covariance of y1
nlag=128;
c1=corr(y1,nlag);
c1=c1';

//Compute the filter with maximum order=15 and p=1
n=5;
[la1,sig1]=lattn(n,1,c1);
poly(0,"z");

expected = [1, 0.4429775275224619446313];
computed = coeff(la1(1));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [1, -0.7424151347904438535963, 0.6626546432402768971315];
computed = coeff(la1(2));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [1, -0.7483090096409563685143, 0.6686337968594154856916, -0.0048019603017336104020];
computed = coeff(la1(3));
assert_checkalmostequal(computed, expected, 1.e-11);

expected = [1, -2.259216884889600152064, 3.0129744887444243062191, -2.2463220443824241101538, 0.9961140979715100707281];
computed = coeff(la1(4));
assert_checkalmostequal(computed, expected, 1.e-13);

expected = [1, -1.8328746888034248563315, 2.0881496310317460007866, -1.0213063678721847615805, 0.0953432301271536220000, 0.3993857757876373426598];
computed = coeff(la1(5));
assert_checkalmostequal(computed, expected, 1.e-10);

expected = [-1.7888543819998343753497, 1];
computed = coeff(sig1(1));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [1.2273893327241232054092, -1.2451484900822573642643, 1];
computed = coeff(sig1(2));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [-1.5168020192922293531979, 2.3534861083273810322680, -2.2502643910424811046767, 1];
computed = coeff(sig1(3));
assert_checkalmostequal(computed, expected, 1.e-13);

expected = [1.0674946929328583689056, -2.3156179157577936322809, 3.0672491379903537023210, -2.255390458180256185727, 1];
computed = coeff(sig1(4));
assert_checkalmostequal(computed, expected, 1.e-12);

expected = [1.4523203295727387107661, -2.2136119179063014072995, 2.0601861867297621699890, -0.1951300338336365847169, -0.8087137031222213501280, 1];
computed = coeff(sig1(5));
assert_checkalmostequal(computed, expected, 1.e-10);

//Compare result of poles with p=-1 and with levin function
[la2,sig2]=lattn(n,-1,c1);
expected = [1, -1.7888543819998343753497];
computed = coeff(la2(1));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [1, -1.2451484900822573642643, 1.2273893327241232054092];
computed = coeff(la2(2));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [1, -2.25026439104248154877, 2.3534861083273810322680, -1.5168020192922295752425];
computed = coeff(la2(3));
assert_checkalmostequal(computed, expected, 1.e-13);

expected = [1, -2.2553904581802566298165, 3.0672491379903545904995, -2.3156179157577940763701, 1.0674946929328590350394];
computed = coeff(la2(4));
assert_checkalmostequal(computed, expected, 1.e-12);

expected = [1, -0.8087137031221993677121, -0.1951300338336872108869, 2.0601861867298310038166, -2.2136119179063538098262, 1.4523203295727622474942];
computed = coeff(la2(5));
assert_checkalmostequal(computed, expected, 1.e-10);

expected = [0.4429775275224624442316, 1];
computed = coeff(sig2(1));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [0.6626546432402768971315, -0.7424151347904438535963, 1];
computed = coeff(sig2(2));
assert_checkalmostequal(computed, expected, 1.e-14);

expected = [-0.0048019603017336104020, 0.6686337968594154856916, -0.7483090096409563685143, 1];
computed = coeff(sig2(3));
assert_checkalmostequal(computed, expected, 1.e-11);

expected = [0.9961140979715094045943, -2.2463220443824232219754, 3.0129744887444229739515, -2.2592168848895997079751, 1];
computed = coeff(sig2(4));
assert_checkalmostequal(computed, expected, 1.e-13);

expected = [0.3993857757876351222137, 0.0953432301271577298252, -1.0213063678721903126956, 2.0881496310317495535003, -1.8328746888034264106437, 1];
computed = coeff(sig2(5));
assert_checkalmostequal(computed, expected, 1.e-10);
