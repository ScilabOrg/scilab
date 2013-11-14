// Copyright (C) 2012-2013 - Michael Baudin
// Copyright (C) 2010 - INRIA - Michael Baudin
//
// This file must be used under the terms of the GNU Lesser General Public License license :
// http://www.gnu.org/copyleft/lesser.html

// Run with test_run('statistics', 'cov', ['no_check_error_output']);

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

// Check error
assert_checkfalse(execstr("cov()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"),"cov", 1, 2, 3);
assert_checkerror("cov()", refMsg);

assert_checkfalse(execstr("cov(""r"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1);
assert_checkerror("cov(""r"")", refMsg);

assert_checkfalse(execstr("cov([1;2], ""r"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: an integer or a real matrix expected.\n"),"cov", 2);
assert_checkerror("cov([1;2], ""r"")", refMsg);

assert_checkfalse(execstr("cov(""r"", [1;2])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1);
assert_checkerror("cov(""r"", [1;2])", refMsg);

assert_checkfalse(execstr("cov(""r"", [1;2], 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 1);
assert_checkerror("cov(""r"", [1;2], 1)", refMsg);

assert_checkfalse(execstr("cov([1;2], ""r"", 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"),"cov", 2);
assert_checkerror("cov([1;2], ""r"", 1)", refMsg);

assert_checkfalse(execstr("cov([1;2], [3;4], ""r"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: an integer expected.\n"),"cov", 3);
assert_checkerror("cov([1;2], [3;4], ""r"")", refMsg);

x = [1;2];
y = [3;4];
computed = cov (x,y);
expected = [0.5,0.5;0.5,0.5];
assert_checkequal ( computed , expected );
//
// The same, with nrmlztn=0
x = [1;2];
y = [3;4];
computed = cov (x,y,0);
expected = [0.5,0.5;0.5,0.5];
assert_checkequal ( computed , expected );
//
x = [230;181;165;150;97;192;181;189;172;170];
y = [125;99;97;115;120;100;80;90;95;125];
expected = [1152.4556,-88.911111;-88.911111,244.26667];
computed = cov (x,y);
assert_checkalmostequal ( computed , expected , 1.e-7 );
//
// The same, with nrmlztn=0
x = [230;181;165;150;97;192;181;189;172;170];
y = [125;99;97;115;120;100;80;90;95;125];
expected = [1152.4556,-88.911111;-88.911111,244.26667];
computed = cov (x,y,0);
assert_checkalmostequal ( computed , expected , 1.e-7 );
//
x = [1;2;3;4;5];
computed = cov (x);
expected = 2.5;
assert_checkequal ( computed , expected );
//
// The same, with nrmlztn=0
x = [1;2;3;4;5];
computed = cov (x,0);
expected = 2.5;
assert_checkequal ( computed , expected );
//
A = [-1 1 2 ; -2 3 1 ; 4 0 3];
Cexpected = [
   10.3333   -4.1667    3.0000
   -4.1667    2.3333   -1.5000
    3.0000   -1.5000    1.0000
];
C = cov (A);
assert_checkalmostequal ( Cexpected , C , [] , 1.e-4, "element");
//
// The same, with nrmlztn=0
A = [-1 1 2 ; -2 3 1 ; 4 0 3];
Cexpected = [
   10.3333   -4.1667    3.0000
   -4.1667    2.3333   -1.5000
    3.0000   -1.5000    1.0000
];
C = cov(A,0);
assert_checkalmostequal ( Cexpected , C , [] , 1.e-4, "element");
//
// Reference
// 6.5.4.1. Mean Vector and Covariance Matrix
// http://www.itl.nist.gov/div898/handbook/pmc/section5/pmc541.htm
A = [
4.0 2.0 0.60
4.2 2.1 0.59
3.9 2.0 0.58
4.3 2.1 0.62
4.1 2.2 0.63
];
S = [
0.025 0.0075 0.00175
0.0075 0.007 0.00135
0.00175 0.00135 0.00043
];
C = cov (A);
assert_checkalmostequal ( S , C , 10*%eps , [] , "element");
//
// The same, with nrmlztn=0
A = [
4.0 2.0 0.60
4.2 2.1 0.59
3.9 2.0 0.58
4.3 2.1 0.62
4.1 2.2 0.63
];
S = [
0.025 0.0075 0.00175
0.0075 0.007 0.00135
0.00175 0.00135 0.00043
];
C = cov (A,0);
assert_checkalmostequal ( S , C , 10*%eps , [] , "element");
//
x = [1;2];
computed = cov (x,1);
expected = 0.25;
assert_checkequal ( computed , expected );
//
x = [1;2];
computed = cov (x,0);
expected = 0.5;
assert_checkequal ( computed , expected );
//
x = [1;2];
y = [3;4];
computed = cov (x,y,1);
expected = [0.25,0.25;0.25,0.25];
assert_checkequal ( computed , expected );
//
// Matlab compatibility
x=[-1 1 2 ; -2 3 1 ; 4 0 3];
computed = cov (x);
expected = [
   10.3333   -4.1667    3.0000
   -4.1667    2.3333   -1.5000
    3.0000   -1.5000    1.0000
];
assert_checkalmostequal ( computed , expected ,[],1.e-4);
