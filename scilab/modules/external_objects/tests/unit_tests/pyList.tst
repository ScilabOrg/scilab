// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

l = pyList(1, 2+%i*3, "Hello", [1 2;3 4], [%t; %f]);

pyAddVar("l",l)

pyEvalStr("lref=[1.0, (2+3j), ''Hello'', [[1.0, 2.0], [3.0, 4.0]], [[True], [False]]]")
pyEvalStr("isEqual=(l==lref)")

assert_checktrue(pyUnwrap(pyGetVar("isEqual")));
assert_checkerror("pyList(sprand(100,1000,0.001)", [], 3);