// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 9565 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9565
//
// <-- Short Description -->
// The tolssizedeltafvmethod and toldeltafv options of neldermead are unnecessary.
//


function stop = myoutputcmd ( state, data, tolsimplexizeabsolute, toldeltafv )
    simplex = data.simplex
    ssize = optimsimplex_size ( simplex , "sigmaplus" );
    shiftfv = abs(optimsimplex_deltafvmax( simplex ))
    stop = %f
    if ( state == "iter") then
        if ( ( ssize < tolsimplexizeabsolute ) & ( shiftfv < toldeltafv ) ) then
            stop = %t;
        end
    end
endfunction

function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test with my own termination criteria 
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-maxiter",%inf);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-method","variable");
// Disable default terminations
nm = neldermead_configure(nm,"-tolxmethod",%f);
nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
tolsimplexizeabsolute = 1.e-4;
toldeltafv = 1.e-4;
stopfun = list(myoutputcmd,tolsimplexizeabsolute,toldeltafv);
nm = neldermead_configure(nm,"-outputcommand",stopfun);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0 1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , [] , 1e-4 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "userstop" );
// Check simplex size
simplex = neldermead_get(nm,"-simplexopt");
ssize = optimsimplex_size ( simplex , "sigmaplus" );
assert_checkequal ( ssize<1.e-4 , %t );
// Check function value shift
shiftfv = abs(optimsimplex_deltafvmax( simplex ));
assert_checkequal ( shiftfv<1.e-4 , %t );
// Check function evaluations
funevals = neldermead_get(nm,"-funevals");
assert_checkequal ( funevals<200 , %t );
nm = neldermead_destroy(nm);
