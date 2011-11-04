// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// Basic use
computed=logspace(0,1,11);
expected=[0. 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.];
expected=[1,1.2589254,1.5848932,1.9952623,2.5118864,3.1622777,3.9810717,5.0118723,6.3095734,7.9432823,10]   ;
assert_checkalmostequal(computed,expected);
// Basic use with 12 and 14 which also test scaling

// Special cases
if logspace(0,1,1)<>10 then pause; end
if logspace(0,1,-1)<>10 then pause; end
if logspace(0,1,0)<>10 then pause; end
if logspace(0,10,1)<>1.000D+10 then pause; end
if logspace(0,10,-1)<>1.000D+10 then pause; end
if logspace(0,10,0)<>1.000D+10 then pause; end
if logspace(10,1,1)<>10 then pause; end
if logspace(10,1,-1)<>10 then pause; end
if logspace(10,1,0)<>10 then pause; end

// Vector input
assert_checkequal(size(logspace([0;2],[2;5],5)),[2,5]);
assert_checkequal(size(logspace([0;2],[2;5],2)),[2,2]);
assert_checkequal(logspace([0;2],[2;5],2),[0,2;2,5]);
assert_checkalmostequal(logspace([-1;2],[2;6],6), [-1,-0.4,0.2,0.8,1.4,2; 2,2.8,3.6,4.4,5.2,6]);

// Perform a check on the size of the input arguments
assert_checkequal(execstr('logspace(2,[2,2])','errcatch'), 10000);
assert_checktrue(lasterror() <> []);
