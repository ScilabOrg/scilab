// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9208 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9208
//
// <-- Short Description -->
// Added three optional output arguments to optim(), to retrieve #iterations,
// #evaluations and a termination flag.
//

// Example 1
a  = 1.0;
b  = 2.0;
c  = 3.0;
d  = 4.0;
x0 = [1 1];
function [f, g, ind] = costfunction(x, ind, a, b, c, d)
    f = a * ( x(1) - c ) ^2 + b * ( x(2) - d )^2;
    g(1) = 2 * a * ( x(1) - c );
    g(2) = 2 * b * ( x(2) - d );
endfunction
costf = list ( costfunction , a , b , c, d );

[fopt, xopt, w, g, iters, evals, err] = optim ( costf , x0 );
assert_checkequal([iters evals err], [10 11 1]);

[fopt, xopt, w, g, iters, evals, err] = optim ( costf , x0 , "ar",nap=5 );
assert_checkequal([iters evals err], [4 5 4]);

[fopt, xopt, w, g, iters, evals, err] = optim ( costf , x0 , "ar",nap=100,iter=5 );
assert_checkequal([iters evals err], [6 7 5]);


// Example 2
x0 = [-1.2 1.0];
function f = rosenbrock(x)
    f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction
function [f, g, ind] = rosenbrockCost2(x, ind)
    f = rosenbrock ( x );
    g = derivative ( rosenbrock , x.' , order = 4 );
endfunction

[fopt, xopt, w, g, iters, evals, err] = optim ( rosenbrockCost2 , x0 );
assert_checkequal([iters evals err], [37 50 9]);

[fopt, xopt, w, g, iters, evals, err] = optim ( rosenbrockCost2 , x0 , "ar",nap=10 );
assert_checkequal([iters evals err], [8 10 4]);

[fopt, xopt, w, g, iters, evals, err] = optim ( rosenbrockCost2 , x0 , "ar",nap=100,iter=10 );
assert_checkequal([iters evals err], [11 14 5]);
