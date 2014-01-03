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
0.000257209601
0.001028838404
0.005144192018
0.009259545632
0.027007008092
0.073561945850
0.128604800438
0.205767680700
0.303764538634
0.364723214041
0.397131623751
0.363179956436
0.289875220186
0.190335104648
0.110085709175
0.061215885008
0.026749798491
0.010288384035
0.002829305610
0.001286048004 ]';
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
refCF = [0.09982 0.10018];
assert_checkequal(cfC, refCF);
cfC = histc(int8([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int16([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int32([-5 0 5]), d);
assert_checkequal(cfC, refCF);
assert_checkequal(sum(cfC), 0.2);
cfC = histc([-5 0 5], d, %f);
assert_checkequal(sum(cfC), 10000); // -5 > d > 5, and d has 10000 elements.



lambda = 2;
grand("setsd", 0);
D = grand(100000, 1, "exp", 1/lambda);
[cfC, indC] = histc(40, D);

refCF = [
1.698711801698
1.24408608389
0.908431380900
0.656611502242
0.489635746028
0.352875852007
0.257181774872
0.194479129735
0.141490978915
0.097271105433
0.073615680960
0.050401824411
0.038416409344
0.028512671632
0.021447584856
0.014634822607
0.010660711296
0.008200547151
0.005866545269
0.004289516971
0.004163354707
0.001387784902
0.001450866034
0.000946216979
0.000756973583
0.000756973583
0.000441567923
0.000315405660
0.000252324528
0.000189243396
0.000063081132
0.000189243396
0.000126162264
0.000063081132
0.000063081132
0
0
0.000063081132
0
0.000063081132 ]';
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
refCF = 0.142857142857;
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
