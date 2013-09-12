// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

//
// Test the call to the cost function
//


// Test when there is no cost function
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
cmd = "[ opt , f , index ] = optimbase_function ( opt , [0.0 0.0] , 1 );";
assert_checkerror(cmd,"%s: Empty function (use -function option).",[],"optimbase_function");
opt = optimbase_destroy(opt);

// Test simple case
function [ y , index ] = rosenbrock ( x , index )
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "rosenbrock", 2);
        error(errmsg)
    end
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

opt = optimbase_new ();
funevals = optimbase_get ( opt , "-funevals" );
assert_checkequal ( funevals , 0  );
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function",rosenbrock);
[ opt , f , index ] = optimbase_function ( opt , [0.0 0.0] , 1 );
assert_checkalmostequal ( f , 1.0 , %eps );
assert_checkequal ( index , 1  );
funevals = optimbase_get ( opt , "-funevals" );
assert_checkequal ( funevals , 1  );
opt = optimbase_destroy(opt);

// Test simple case where the index is changed
function [ y , index ] = rosenbrock0 ( x , index )
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "rosenbrock0", 2);
        error(errmsg)
    end
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
  index = 0;
endfunction

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function",rosenbrock0);
[ opt , f , index ] = optimbase_function ( opt , [0.0 0.0] , 1 );
assert_checkalmostequal ( f , 1.0 , %eps );
assert_checkequal ( index , 0  );
opt = optimbase_destroy(opt);


//
// Test with an additional argument
// In this case, the mydata variable is passed
// explicitely by the optimization class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ y , index ] = rosenbrock2 ( x , index , mydata )
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "rosenbrock2", 3);
        error(errmsg)
    end
  a = mydata.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
endfunction

mystuff = tlist(["T_MYSTUFF","a"]);
mystuff.a = 12.0;

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function",list(rosenbrock2,mystuff));
[ opt , f , index ] = optimbase_function ( opt , [0.0 0.0] , 1 );
assert_checkalmostequal ( f , 144. , %eps );
assert_checkequal ( index , 1 );
opt = optimbase_destroy(opt);

//
// Test with non linear constraints : there is an index
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
// Note
//  The following protocol is used
//  * if index=2, returns f, the value of the cost 
//    function
//  * if index=5, returns c, the value of the nonlinear inequality 
//    constraints, as a row array
//  * if index=6, returns f and c
//  The inequality constraints are expected to be positive.
//
function [ f , c , index ] = optimtestcase ( x , index )
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimtestcase", 2);
        error(errmsg)
    end
    if ( lhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of output argument: %d expected.\n"), "optimtestcase", 3);
        error(errmsg)
    end
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction
x0 = [0.0 0.0 0.0 0.0].';
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt= optimbase_configure(opt,"-function",optimtestcase);
opt= optimbase_configure(opt,"-nbineqconst",3);
[this,f,c,index] = optimbase_function ( opt , x0 , 2 );
assert_checkequal ( index , 2 );
assert_checkalmostequal ( f , 0. , %eps );
assert_checkequal ( c , [] );
[this,f,c,index] = optimbase_function ( opt , x0 , 5 );
assert_checkequal ( index , 5 );
assert_checkequal ( f , [] );
assert_checkalmostequal ( c , [8.    10.    5.] , %eps );
[this,f,c,index] = optimbase_function ( opt , x0 , 6 );
assert_checkequal ( index , 6 );
assert_checkalmostequal ( f , 0. , %eps );
assert_checkalmostequal ( c , [8.    10.    5.] , %eps );
opt = optimbase_destroy(opt);


//
// Test when there are both nonlinear constraints and a customized data
// boxproblemA --
//   Computes the Box problem A cost function and 
//   inequality constraints.
//
// Arguments
//   x: the point where to compute the function
//   index : what to compute
//     if index=2, returns f
//     if index=5, returns c
//     if index=6, returns f and c
//   data : the parameters of Box cost function
// Note
//  The inequality constraints are expected to be positive.
//
function [ f , c , index ] = boxproblemA ( x , index , data )
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "boxproblemA", 3);
        error(errmsg)
    end
    if ( lhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of output argument: %d expected.\n"), "boxproblemA", 3);
        error(errmsg)
    end
  f = []
  c = []
  b = x(2) + 0.01 * x(3)
  x6 = (data.k1 + data.k2 * x(2) ...
            + data.k3 * x(3) + data.k4 * x(4) + data.k5 * x(5)) * x(1)
  y1 = data.k6 + data.k7 * x(2) + data.k8 * x(3) ...
            + data.k9 * x(4) + data.k10 * x(5)
  y2 = data.k11 + data.k12 * x(2) + data.k13 * x(3) ...
            + data.k14 * x(4) + data.k15 * x(5)
  y3 = data.k16 + data.k17 * x(2) + data.k18 * x(3) ...
            + data.k19 * x(4) + data.k20 * x(5)
  y4 = data.k21 + data.k22 * x(2) + data.k23 * x(3) ...
            + data.k24 * x(4) + data.k25 * x(5)
  x7 = ( y1 + y2 + y3 ) * x(1)
  x8 = (data.k26 + data.k27 * x(2) + data.k28 * x(3) ...
            + data.k29 * x(4) ...
            + data.k30 * x(5) ) * x(1) + x6 + x7
  if ( index==2 | index==6 ) then
    f = (data.a2 * y1 + data.a3 * y2 + data.a4 * y3 + data.a5 * y4 ...
             + 7840 * data.a6 - 100000 * data.a0 ...
             - 50800 * b * data.a7 + data.k31 + data.k32 * x(2) + data.k33 * x(3) ...
             + data.k34 * x(4) + data.k35 * x(5)) * x(1) ...
             - 24345 + data.a1 * x6
    f = -f
  end
  if ( index==5 | index==6 ) then
      c1 = x6
      c2 = 294000 - x6
      c3 = x7
      c4 = 294000 - x7
      c5 = x8
      c6 = 277200 - x8
      c = [c1 c2 c3 c4 c5 c6]
  end
endfunction

boxparams = struct();
boxparams.a0 = 9;
boxparams.a1 = 15;
boxparams.a2 = 50;
boxparams.a3 = 9.583;
boxparams.a4 = 20;
boxparams.a5 = 15;
boxparams.a6 = 6;
boxparams.a7 = 0.75;
boxparams.k1 = -145421.402;
boxparams.k2 = 2931.1506;
boxparams.k3 = -40.427932;
boxparams.k4 = 5106.192;
boxparams.k5 = 15711.36;
boxparams.k6 = -161622.577;
boxparams.k7 = 4176.15328;
boxparams.k8 = 2.8260078;
boxparams.k9 = 9200.476;
boxparams.k10 = 13160.295;
boxparams.k11 = -21686.9194;
boxparams.k12 = 123.56928;
boxparams.k13 = -21.1188894;
boxparams.k14 = 706.834;
boxparams.k15 = 2898.573;
boxparams.k16 = 28298.388;
boxparams.k17 = 60.81096;
boxparams.k18 = 31.242116;
boxparams.k19 = 329.574;
boxparams.k20 = -2882.082;
boxparams.k21 = 74095.3845;
boxparams.k22 = -306.262544;
boxparams.k23 = 16.243649;
boxparams.k24 = -3094.252;
boxparams.k25 = -5566.2628;
boxparams.k26 = -26237.0;
boxparams.k27 = 99.0;
boxparams.k28 = -0.42;
boxparams.k29 = 1300.0;
boxparams.k30 = 2100.0;
boxparams.k31 = 925548.252;
boxparams.k32 = -61968.8432;
boxparams.k33 = 23.3088196;
boxparams.k34 = -27097.648;
boxparams.k35 = -50843.766;
x0 = [2.52 2.0 37.5 9.25 6.8].';

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",5);
opt= optimbase_configure(opt,"-function",list(boxproblemA,boxparams));
opt = optimbase_configure(opt,"-nbineqconst",6);
[this,f,c,index] = optimbase_function ( opt , x0 , 2 );
assert_checkequal ( index , 2 );
assert_checkalmostequal ( f , -2351243.5  , 1.e-7 );
assert_checkequal ( c , [] );
[this,f,c,index] = optimbase_function ( opt , x0 , 5 );
assert_checkequal ( index , 5 );
assert_checkequal ( f , [] );
assert_checkalmostequal ( c , [32745.827    261254.17    96991.969    197008.03    130368.43    146831.57] , 1.e-7 );
[this,f,c,index] = optimbase_function ( opt , x0 , 6 );
assert_checkequal ( index , 6 );
assert_checkalmostequal ( f , -2351243.5 , 1.e-7 );
assert_checkalmostequal ( c , [    32745.827    261254.17    96991.969    197008.03    130368.43    146831.57] , 1.e-7 );
opt = optimbase_destroy(opt);

//
// Test with an additional argument.
//
function [ y , index ] = rosenbrock3 ( x , index , mydata )
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "rosenbrock3", 3);
        error(errmsg)
    end
    if ( lhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of output argument: %d expected.\n"), "rosenbrock3", 2);
        error(errmsg)
    end
  y = 100*(x(2)-x(1)^2)^2 + ( 1.0 - x(1))^2;
endfunction

mystuff = tlist(["T_MYSTUFF","n"]);
mystuff.n = 0;

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function",list(rosenbrock3,mystuff));
[ opt , f , index ] = optimbase_function ( opt , [0.0 0.0] , 2 );
assert_checkequal ( index , 2 );
assert_checkalmostequal ( f , 1.0 , %eps );
opt = optimbase_destroy(opt);

//
// rosenbrock --
//   Defines the Rosenbrock test function.
// Arguments
//    x : the current point
//    index : a flag which indicates what to compute
//      if index=1, output a message
//      if index=2, compute f
//      if index=3, compute g
//      if index=4, compute f and g
// Note : 
//   This function could be accepted by optim, hence the test.
//
function [ f , g , index ] = rosenbrock4 ( x , index )
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "rosenbrock4", 2);
        error(errmsg)
    end
    if ( lhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of output argument: %d expected.\n"), "rosenbrock4", 3);
        error(errmsg)
    end
  if index == 1 then
    mprintf ( "index = %d, x = [%f %f]\n" , index , x(1) , x(2) );
  end
  f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
  g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
  g(2) = 200. * ( x(2) - x(1)**2 )
endfunction

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt= optimbase_configure(opt,"-function", rosenbrock4 );
opt = optimbase_configure(opt,"-withderivatives",%t);
[ opt , f , g , index ] = optimbase_function ( opt , [-1.2 1.0] , 4 );
assert_checkequal ( index , 4 );
assert_checkalmostequal ( f , 24.2 , %eps );
assert_checkalmostequal ( g , [-215.6 -88.0].' , %eps );
opt = optimbase_destroy(opt);

