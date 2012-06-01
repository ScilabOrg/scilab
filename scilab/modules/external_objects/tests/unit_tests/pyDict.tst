// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

d = pyDict(1, 2+%i*3, "Hello", %t, %f, [1 2;3 4])

pyAddVar("d",d)

pyEvalStr("dref={False: [[1.0, 2.0], [3.0, 4.0]], 1.0: (2+3j), ''Hello'': True}")
pyEvalStr("isEqual=(d==dref)")

assert_checktrue(pyUnwrap(pyGetVar("isEqual")));
assert_checkerror("pyDict(sprand(100,1000,0.001)", [], 3);