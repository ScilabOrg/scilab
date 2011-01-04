// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
    if expected==0.0 then
        shift = norm(computed-expected);
    else
        shift = norm(computed-expected)/norm(expected);
    end
    if shift < epsilon then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
    if computed==expected then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction


// With "karmarkar"
// With slack variables:
//
// Min { -20.x1 - 24.x2 } such as:
// 3.x1 + 6.x2 + x3 = 60
// 4.x1 + 2.x2 + x4 = 32
// x1, x2, x3, x4 >= 0
c = [-20 -24 0 0]';
Aeq = [
3 6 1 0
4 2 0 1
];
beq = [60 32]';
xexpected = [4 8 0 0]';
fexpected = -272;
// [x0,kerA]=linsolve(Aeq,-beq)
// x0 = lsq(Aeq,beq) also works
x0 = [
4.1128205
7.7333333
1.2615385
0.0820513
];
[xopt,fopt]=karmarkar(Aeq,beq,c,x0);
assert_close ( xopt , xexpected , 1.e-3 );
assert_close ( fopt , fexpected , 1.e-4 );
//
// Configure the relative tolerance
[xopt,fopt]=karmarkar(Aeq,beq,c,x0,1.e-6);
assert_close ( xopt , xexpected , 1.e-4 );
assert_close ( fopt , fexpected , 1.e-5 );
//
// Configure the gamma
[xopt,fopt]=karmarkar(Aeq,beq,c,x0,1.e-6,0.1);
assert_close ( xopt , xexpected , 1.e-3 );
assert_close ( fopt , fexpected , 1.e-4 );
//
// Check new API (from Scilab v5.3.x).
//
// Check number of iterations
[xopt,fopt,count]=karmarkar(Aeq,beq,c,x0);
assert_close ( xopt , xexpected , 1.e-3 );
assert_close ( fopt , fexpected , 1.e-4 );
assert_equal ( count>10 , %t );
//
// Check number of iterations, with default options
[xopt,fopt,count]=karmarkar(Aeq,beq,c,x0,[],[],10);
assert_equal ( count , 10 );
//
// Check output function
function stop = myoutputfunction ( iter , xopt , fopt , d )
    norstep = norm(d)
    localmsg = gettext("Iteration #%3.0f, fopt=%10.3e, ||d||=%10.3e\n")
    mprintf(localmsg,iter,fopt,norstep)
    stop = %f
endfunction
xopt=karmarkar(Aeq,beq,c,x0,[],[],[],myoutputfunction);
assert_close ( xopt , xexpected , 1.e-3 );
//
// Check output function with additionnal arguments
function stop = myoutputfunction2 ( iter , xopt , fopt , d , myAeq , mybeq , myc )
    norstep = norm(d)
    localmsg = gettext("Iteration #%3.0f, fopt=%10.3e, ||d||=%10.3e, ||Ax-beq||=%.3e\n")
    mprintf(localmsg,iter,fopt,norstep,norm(myAeq*xopt-mybeq))
    stop = %f
endfunction
xopt=karmarkar(Aeq,beq,c,x0,[],[],[],list(myoutputfunction2,Aeq,beq,c));
assert_close ( xopt , xexpected , 1.e-3 );

