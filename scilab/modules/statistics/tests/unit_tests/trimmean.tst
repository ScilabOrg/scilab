//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// With x as a vector and 1 argument
data = [10, 20, 30, 40, 50, 60, 70, 80, 4000];
computed = trimmean(data);
expected = 50;
if abs(computed-expected)>%eps then pause,end
// With x as a vector and 2 arguments
data = [10, 20, 30, 40, 50, 60, 70, 80, 4000];
computed = trimmean(data,25);
expected = 50;
if abs(computed-expected)>%eps then pause,end
// With x as a matrix and 2 arguments
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,25);
expected = 45;
if abs(computed-expected)>%eps then pause,end
// With x as a vector and specified orient
data = [10, 20, 30, 40, 50, 60, 70, 80, 4000];
computed = trimmean(data,25,"all");
expected = 50;
if abs(computed-expected)>%eps then pause,end
// With x as a matrix and specified orient
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,25,"all");
expected = 45;
if abs(computed-expected)>%eps then pause,end
// With x as a vector and non-positionnal orien="all"
data = [10, 20, 30, 40, 50, 60, 70, 80, 4000];
computed = trimmean(data,25,orien="all");
expected = 50;
if abs(computed-expected)>%eps then pause,end
// With x as a matrix and non-positionnal orien="all"
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,25,orien="all");
expected = 45;
if abs(computed-expected)>%eps then pause,end
// With x as a matrix and positionnal orien="r"
data = [
10 10 10 10
20 20 20 20
30 30 30 30
4000 4000 4000 4000
];
computed = trimmean(data,50,"r");
expected = [25,25,25,25];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and positionnal orien=1
data = [
10 10 10 10
20 20 20 20
30 30 30 30
4000 4000 4000 4000
];
computed = trimmean(data,50,1);
expected = [25,25,25,25];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and positionnal orien="c"
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,50,"c");
expected = [25;65];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and positionnal orien=2
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,50,2);
expected = [25;65];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and non-positionnal orien="r"
data = [
10 10 10 10
20 20 20 20
30 30 30 30
4000 4000 4000 4000
];
computed = trimmean(data,orien="r",50);
expected = [25,25,25,25];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and non-positionnal orien=1
data = [
10 10 10 10
20 20 20 20
30 30 30 30
4000 4000 4000 4000
];
computed = trimmean(data,orien="r",50);
expected = [25,25,25,25];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and non-positionnal orien="c"
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,orien="c",50);
expected = [25;65];
if norm(computed-expected)>%eps then pause,end
// With x as a matrix and non-positionnal orien=2
data = [
10 20 30 40
50 60 70 4000
];
computed = trimmean(data,orien=2,50);
expected = [25;65];
if norm(computed-expected)>%eps then pause,end
// Test extremal values of discard.
data = [10, 20, 30, 40, 50, 60, 70, 80, 90];
computed = trimmean(data,discard=0);
expected = 50;
if abs(computed-expected)>%eps then pause,end
// Test extremal values of discard.
data = [10, 20, 30, 40, 50, 60, 70, 80, 4000];
computed = trimmean(data,discard=100);
expected = 50;
if abs(computed-expected)>%eps then pause,end

