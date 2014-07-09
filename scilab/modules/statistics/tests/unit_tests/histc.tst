// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

rand("seed", 0);
d = rand(1, 10000, "normal");
[cfC, indC] = histc(20, d);

refCF = [
0.0001
0.0004
0.002
0.0036
0.0105
0.0286
0.05
0.08
0.1181
0.1418
0.1544
0.1412
0.1127
0.074
0.0428
0.0238
0.0104
0.004
0.0011
0.0005 ]';
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF);
[cfC, indC] = histc(int32(20), d);
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF);

// Same test, without normalization
[cfC, indC] = histc(20, d, normalization=%f);

refCF = [
1
4
20
36
105
286
500
800
1181
1418
1544
1412
1127
740
428
238
104
40
11
5 ]';
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);
[cfC, indC] = histc(int16(20), d, normalization=%f);
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);


// With x instead of n as first argument
cfC = histc([-5 0 5], d);
refCF = [0.4991 0.5009];
assert_checkequal(cfC, refCF);
cfC = histc(int8([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int16([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int32([-5 0 5]), d);
assert_checkequal(cfC, refCF);
assert_checkequal(sum(cfC), 1);
cfC = histc([-5 0 5], d, %f);
assert_checkequal(sum(cfC), 10000); // -5 > d > 5, and d has 10000 elements.



lambda = 2;
grand("setsd", 0);
D = grand(100000, 1, "exp", 1/lambda);
[cfC, indC] = histc(40, D);

refCF = [
0.26929
0.19722
0.14401
0.10409
0.07762
0.05594
0.04077
0.03083
0.02243
0.01542
0.01167
0.00799
0.00609
0.00452
0.0034
0.00232
0.00169
0.0013
0.00093
0.00068
0.00066
0.00022
0.00023
0.00015
0.00012
0.00012
0.00007
0.00005
0.00004
0.00003
0.00001
0.00003
0.00002
0.00001
0.00001
0
0
0.00001
0
0.00001 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);
[cfC, indC] = histc(int8(40), D);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);

// Same test, without normalization
[cfC, indC] = histc(40, D, normalization=%f);

refCF = [
26929
19722
14401
10409
7762
5594
4077
3083
2243
1542
1167
799
609
452
340
232
169
130
93
68
66
22
23
15
12
12
7
5
4
3
1
3
2
1
1
0
0
1
0
1 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);
[cfC, indC] = histc(int32(40), D, normalization=%f);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);

// With x instead of n as first argument
cfC = histc([0 7], D);
refCF = 1;
assert_checkalmostequal(cfC, refCF);
cfC = histc(int8([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int16([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int32([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc([0 7], D, %f);
assert_checkequal(cfC, 100000); // 0 > D > 7, and D has 100000 elements.

// Error checks
refMsg = msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"), "histc", 2);
assert_checkerror("histc(1)", refMsg);
assert_checkerror("histc()", refMsg);

refMsg = msprintf(_("%s: Wrong type of input argument #%d: A single boolean expected.\n"), "histc", 3);
assert_checkerror("histc(1, 1, normalization=[%t %t])", refMsg);
assert_checkerror("histc(1, 1, [%t %t])", refMsg);
assert_checkerror("histc(1, 1, """")", refMsg);
assert_checkerror("histc(1, 1, 1)", refMsg);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"), "histc", 1, "[1, oo)");
assert_checkerror("histc(0, 1, %t)", refMsg);
assert_checkerror("histc(-1, 1)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "histc", 1);
assert_checkerror("histc(1.5, 1, %t)", refMsg);

refMsg = msprintf(_("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), "histc", 1);
assert_checkerror("histc([2 1], 1, %t)", refMsg);
assert_checkerror("histc([1 1], 1, %t)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "histc", 1);
assert_checkerror("histc(list(), 1)", refMsg);
assert_checkerror("histc("""", 1)", refMsg);
assert_checkerror("histc(%t, 1)", refMsg);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "histc", 2);
assert_checkerror("histc(1, list())", refMsg);
assert_checkerror("histc(1, """")", refMsg);
assert_checkerror("histc(1, %t)", refMsg);
