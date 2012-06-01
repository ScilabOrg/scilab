// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

pyUseNumpy(%f)
a=%pi+%i*%e;

assert_checkerror("s=pyRepr(a)", [], 999);

s=pyRepr(pyWrap(a));
assert_checkequal(s,"(3.141592653589793+2.718281828459045j)");

a=2;
s=pyRepr(pyWrap(a));
assert_checkequal(s,"2.0");

a=[2,2];
s=pyRepr(pyWrap(a));
assert_checkequal(s,"[2.0, 2.0]");

a="aze";
s=pyRepr(pyWrap(a));
assert_checkequal(s,"''aze''");

a=["aze","bze"];
s=pyRepr(pyWrap(a));
assert_checkequal(s,"[''aze'', ''bze'']");

pyUseNumpy(%t)
a=[2,2];
s=pyRepr(pyWrap(a));
assert_checkequal(s,"array([ 2.,  2.])");

a=2;
s=pyRepr(pyWrap(a));
assert_checkequal(s,"2.0");

assert_checkerror("s=pyRepr()", [], 77);
assert_checkerror("s=pyRepr(2,2)", [], 77);
