// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13045 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13045
//
// <-- Short Description -->
//    strchr is slower than strstr

H = ["abc ABC" "bcd" "déf DEF" "hij"]
N = ["c" "c" "é" "k"]
strchr(H', N')
strstr(H', N')
n = 10000;
tic();for i=1:n, strchr(H, N); end;ref1 = toc();
tic();for i=1:n, strstr(H, N); end;ref2 = toc();

//ensure strchr is not more than 20% slower than strstr
assert_checktrue(ref1 * 0.8 < ref2);