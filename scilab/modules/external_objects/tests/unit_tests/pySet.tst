// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
s = pySet(1, 2+%i*3, "Hello", %t, "Hello", %f)

pyAddVar("s",s)

pyEvalStr("sref=set([False, 1.0, (2+3j), ''Hello''])")

pyEvalStr("isEqual=(s==sref)")

assert_checktrue(pyUnwrap(pyGetVar("isEqual")));

assert_checkerror("pySet(sprand(100,1000,0.001)", [], 3);
