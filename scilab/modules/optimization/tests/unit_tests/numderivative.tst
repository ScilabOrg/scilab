// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


// 1. Test with a scalar argument
function y = myfunction (x)
  y = x*x;
endfunction

x = 1.0;
expected = 2.0;
// 1.1 With default parameters
computed = numderivative(myfunction,x);
assert_checkalmostequal ( computed , expected , 1.e-11 );
// 1.2 Test order 1
computed = numderivative(myfunction,x,[],1);
assert_checkalmostequal ( computed , expected , 1.e-8 );
// 1.3 Test order 2
computed = numderivative(myfunction,x,[],2);
assert_checkalmostequal ( computed , expected , 1.e-11 );
// 1.4 Test order 4
computed = numderivative(myfunction,x,[],4);
assert_checkalmostequal ( computed , expected , 1.e-13 );

// 1.5 Compute second numderivative at the same time
Jexpected = 2.0;
Hexpected = 2.0;
[Jcomputed , Hcomputed] = numderivative(myfunction,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-11 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 1.6 Test order 1
[Jcomputed , Hcomputed] = numderivative(myfunction,x,[],1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-8 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
// 1.7 Test order 2
[Jcomputed , Hcomputed] = numderivative(myfunction,x,[],2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-11 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 1.8 Test order 4
[Jcomputed , Hcomputed] = numderivative(myfunction,x,[],4);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-13 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-11 );

// 1.9 Configure the step
computed = numderivative(myfunction,x,1.e-1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-13 );
// 1.10 Configure the step
[Jcomputed , Hcomputed] = numderivative(myfunction,x,1.e-1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-13 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-11 );

// 2. Test with a vector argument
function y = myfunction2 (x)
  y = x(1)*x(1) + x(2)+ x(1)*x(2);
endfunction
x = [1.0;2.0];
expected = [4.0 2.0];
// 2.1 With default parameters
computed = numderivative(myfunction2,x);
assert_checkalmostequal ( computed , expected , 1.e-10 );
// 2.2 Test order 1
computed = numderivative(myfunction2,x,[],1);
assert_checkalmostequal ( computed , expected , 1.e-8 );
// 2.3 Test order 2
computed = numderivative(myfunction2,x,[],2);
assert_checkalmostequal ( computed , expected , 1.e-10 );
// 2.4 Test order 4
computed = numderivative(myfunction2,x,[],4);
assert_checkalmostequal ( computed , expected , 1.e-13 );

// 2.5 Compute second numderivative at the same time
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 1.0 0];
[Jcomputed , Hcomputed] = numderivative(myfunction2,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 2.6 Test order 1
[Jcomputed , Hcomputed] = numderivative(myfunction2,x,[],1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-8 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-5 );
// 2.7 Test order 2
[Jcomputed , Hcomputed] = numderivative(myfunction2,x,[],2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 2.8 Test order 4
[Jcomputed , Hcomputed] = numderivative(myfunction2,x,[],4);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-13 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-10, 1.e-10 );
// 2.9 Configure the step
// TODO : fix this.
[Jcomputed , Hcomputed] = numderivative(myfunction2,x,1.e-1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-13 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-10, 1.e-10 );

// 3. Test H_form
// 3.1 Test H_form="default"
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 1.0 0.0];
[Jcomputed , Hcomputed] = numderivative(myfunction2 , x , [], [], "default");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 3.2 Test H_form="hypermat"
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0
1.0 0.0];
[Jcomputed , Hcomputed] = numderivative(myfunction2 , x , [], [], "hypermat");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 3.3 Test H_form="blockmat"
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 
1.0 0.0];
[Jcomputed , Hcomputed] = numderivative(myfunction2 , x , [], [], "blockmat");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );

// 5. Test h parameter
// Test a case where the default step h is very small ~ 1.e-9,
// but, because the function is very flat in the neighbourhood of the 
// point, a larger step ~ 1.e-4 reduces the error.
// This means that this test cannot pass if the right step is 
// not taken into account, therefore testing the feature "h is used correctly".
myn = 1.e5;
function y = myfunction3 (x)
  y = x^(2/myn);
endfunction
x = 1.0;
h = 6.055454e-006;
Jexpected = (2/myn) * x^(2/myn-1);
Hexpected = (2/myn) * (2/myn-1) * x^(2/myn-2);
[Jcomputed , Hcomputed] = numderivative(myfunction3 , x , 1.e-4 , 1 );
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-4 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-3 );

// 6. Test Q parameter
// TODO !

//
// 7. Test vector output y
function y=f(x)
  f1 = sin(x(1)*x(2))+exp(x(2)*x(3)+x(1))
  f2 = sum(x.^3)
  y=[f1;f2]
endfunction
// The exact gradient
function [g1,g2] = exactg(x)
  g1(1) = cos(x(1)*x(2))*x(2)+exp(x(2)*x(3)+x(1))
  g1(2) = cos(x(1)*x(2))*x(1)+exp(x(2)*x(3)+x(1))*x(3)
  g1(3) = exp(x(2)*x(3)+x(1))*x(2)
  g2(1) = 3*x(1)^2
  g2(2) = 3*x(2)^2
  g2(3) = 3*x(3)^2
endfunction
// The exact Hessian
function [H1,H2] = exactH(x)
  H1(1,1) = -sin(x(1)*x(2))*x(2)^2+exp(x(2)*x(3)+x(1))
  H1(1,2) = cos(x(1)*x(2)) - sin(x(1)*x(2))*x(2)*x(1)+exp(x(2)*x(3)+x(1))*x(3)
  H1(1,3) = exp(x(2)*x(3)+x(1))*x(2)
  H1(2,1) = H1(1,2)
  H1(2,2) = -sin(x(1)*x(2))*x(1)^2+exp(x(2)*x(3)+x(1))*x(3)^2
  H1(2,3) = exp(x(2)*x(3)+x(1))+exp(x(2)*x(3)+x(1))*x(3)*x(2)
  H1(3,1) = H1(1,3)
  H1(3,2) = H1(2,3)
  H1(3,3) = exp(x(2)*x(3)+x(1))*x(2)^2
  //
  H2(1,1) = 6*x(1)
  H2(1,2) = 0
  H2(1,3) = 0
  H2(2,1) = H2(1,2)
  H2(2,2) = 6*x(2)
  H2(2,3) = 0
  H2(3,1) = H2(1,3)
  H2(3,2) = H2(2,3)
  H2(3,3) = 6*x(3)
endfunction

x=[1;2;3];
[g1,g2] = exactg(x);
[H1,H2] = exactH(x);
Jexpected = [g1';g2'];
Hexpected = [H1(:)';H2(:)'];
// 7.1.1 Check Jacobian with default options
Jcomputed = numderivative(f,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
// 7.1.2 Check Jacobian with order = 1
Jcomputed = numderivative(f,x,[],1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-7 );
// 7.1.3 Check Jacobian with order = 2
Jcomputed = numderivative(f,x,[],2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
// 7.1.4 Check Jacobian with order = 4
Jcomputed = numderivative(f,x,[],4);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
// 7.2.1 Check Jacobian and Hessian with default options
[Jcomputed,Hcomputed]=numderivative(f,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
// 7.2.2 Check Jacobian and Hessian with order = 1
[Jcomputed,Hcomputed]=numderivative(f,x,[],1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-7 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-4 );
// 7.2.3 Check Jacobian and Hessian with order = 2
[Jcomputed,Hcomputed]=numderivative(f,x,[],2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
// 7.2.3 Check Jacobian and Hessian with order = 4
[Jcomputed,Hcomputed]=numderivative(f,x,[],4);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-8 , 1e-11);
//
// 7.3 Test with "blockmat"
Jexpected = [g1';g2'];
Hexpected = [H1;H2];
[Jcomputed,Hcomputed]=numderivative(f,x,[],[],"blockmat");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
//
// 7.4 Test with "hypermat"
Jexpected = [g1';g2'];
Hexpected = [];
Hexpected(:,:,1) = H1;
Hexpected(:,:,2) = H2;
[Jcomputed,Hcomputed]=numderivative(f,x,[],[],"hypermat");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-9 );
assert_checkequal ( size(Hcomputed) , [3 3 2] );
// This is a limitation of assert (http://bugzilla.scilab.org/show_bug.cgi?id=9461)
// assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
assert_checkalmostequal ( Hexpected(:,:,1) , Hexpected(:,:,1) , 1.e-6);
assert_checkalmostequal ( Hexpected(:,:,2) , Hexpected(:,:,2) , 1.e-6);
//
// 8. Count the number of function evaluations
function y = myFevalFun(x)
    global FEVAL
	FEVAL = FEVAL + 1
    y = sum(x.^3)
endfunction
n = 3;
x = ones(n,1);
//
// Jacobian with various orders
global FEVAL;
FEVAL = 0;
g = numderivative(myFevalFun, x, [], 1);
assert_checkequal ( FEVAL, n+1);
//
FEVAL = 0;
g = numderivative(myFevalFun, x, [], 2);
assert_checkequal ( FEVAL, 2*n);
//
FEVAL = 0;
g = numderivative(myFevalFun, x, [], 4);
assert_checkequal ( FEVAL, 4*n);
//
// Hessian with various orders
FEVAL = 0;
[g,H] = numderivative(myFevalFun, x, [], 1);
assert_checkequal ( FEVAL, (n+1)^2+n+1);
//
FEVAL = 0;
[g,H] = numderivative(myFevalFun, x, [], 2);
assert_checkequal ( FEVAL, 4*n^2+2*n);
//
FEVAL = 0;
[g,H] = numderivative(myFevalFun, x, [], 4);
assert_checkequal ( FEVAL, 16*n^2+4*n);
