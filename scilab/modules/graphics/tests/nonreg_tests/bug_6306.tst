// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6306 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6306
//
// <-- Short Description -->
// It was not possible to retrieve the data computed by histplot.
// It is now possible via histcomp().

rand("seed", 0);
d = rand(1, 10000, "normal");
res = histcomp(20, d);

refX = [
-4.0531833
-3.6643953
-3.2756074
-2.8868194
-2.4980314
-2.1092434
-1.7204555
-1.3316675
-0.9428795
-0.5540915
-0.1653036
0.2234844
0.6122724
1.0010604
1.3898483
1.7786363
2.1674243
2.5562123
2.9450002
3.3337882
3.7225762 ];
refY = [
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
5 ];

assert_checkequal(typeof(res), "st");
assert_checkequal(size(res), [1 1]);
assert_checkequal(size(res.x), [1 21]);
assert_checkequal(size(res.y), [1 20]);
assert_checkalmostequal(res.x', refX, [], 1d-6);
assert_checkequal(res.y', refY);




lambda = 2;
X = grand(100000, 1, "exp", 1/lambda);
Xmax = max(X);

refX10 = [
0.0000073
0.1460302
0.2920531
0.4380760
0.5840989
0.7301218
0.8761447
1.0221676
1.1681905
1.3142134 ];
refY10 = [
25532
18779
13983
10433
7871
5943
4395
3244
2538
1856 ];

grand("setsd", 0);
res = histcomp(40, X, style=2);
assert_checkequal(typeof(res), "st");
assert_checkequal(size(res), [1 1]);
assert_checkequal(size(res.x), [1 41]);
assert_checkequal(size(res.y), [1 40]);
assert_checkalmostequal(res.x(1:10)', refX10, [], 1d-5);
assert_checkequal(res.y(1:10)', refY10);
