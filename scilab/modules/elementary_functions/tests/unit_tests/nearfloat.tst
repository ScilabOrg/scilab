// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


// Test typical case: integral significand M in the middle of the min-max range.
// x = 1.5
x = 6755399441055744 * 2^(0-53+1); 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 6755399441055745 * 2^(0-53+1) );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 6755399441055743 * 2^(0-53+1) );
//
// Test at largest M, for "typical case"
p = 53;
// 9007199254740991
largestNormalM = 2^p-1; 
// x = 1.9999999999999997
x = largestNormalM * 2^(0-53+1); 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 2 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 1.9999999999999995 );
//
// Test at minimum M, for "typical case"
p = 53;
// 4503599627370496
smallestNormalM = 2^(p-1);  
// x = 1
x = smallestNormalM * 2^(0-53+1); 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 1.0000000000000002 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , .99999999999999988 );
//
// Test at signed zero
x = +0; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 4.940656458412465D-324 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -4.94065645841246544D-324 );
//
// Test at signed zero
x = -0; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 4.940656458412465D-324 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -4.94065645841246544D-324 );
//
// Test at +%inf
x = %inf;
computed = nearfloat("succ",x);
assert_checkequal ( computed , %inf );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 1.7976931348623157D+308 );
//
// Test at -%inf
x = -%inf;
computed = nearfloat("succ",x);
assert_checkequal ( computed , -1.7976931348623157D+308 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -%inf );
//
// Test at nan
x = %nan;
computed = nearfloat("succ",x);
assert_checkequal ( isnan(computed) , %t );
computed = nearfloat("pred",x);
assert_checkequal ( isnan(computed) , %t );
//
// Test at vmax, the largest normal before overflow
x = 1.7976931348623157D+308;
computed = nearfloat("succ",x);
assert_checkequal ( computed , %inf );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 1.7976931348623155D+308 );
//
// Test at -vmax
x = -1.7976931348623157D+308;
computed = nearfloat("succ",x);
assert_checkequal ( computed , -1.7976931348623155D+308 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -%inf );
//
// Test at vmin, the smallest normal
// x = 2.2250738585072013D-308;
x = 1 * 2^-1022; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 1.0000000000000002 * 2^-1022 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 1.9999999999999995 * 2^-1023 );
//
// Test at vmin, the smallest negative normal
// x = -2.2250738585072013D-308;
x = -1 * 2^-1022; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , -1.9999999999999995 * 2^-1023 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -1.0000000000000002 * 2^-1022 );
//
// Test at alpha, the smallest subnormal before underflow
// x = 4.940656458412465D-324; 
x = 0.0000000000000004440892 * 2^-1023; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 0.0000000000000008881784 * 2^-1023 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , +0 );
//
// Test at -alpha, the smallest negative subnormal before underflow
// x = 4.940656458412465D-324; 
x = -0.0000000000000004440892 * 2^-1023; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 0 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -0.0000000000000008881784 * 2^-1023 );
//
// Test at an "average" subnormal
x = 1.9999D-320; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , 2.0004D-320 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , 1.9997e-320 );
//
// Test at an "average" negative subnormal
x = -1.9999D-320; 
computed = nearfloat("succ",x);
assert_checkequal ( computed , -1.9997e-320 );
computed = nearfloat("pred",x);
assert_checkequal ( computed , -2.0004D-320 );

