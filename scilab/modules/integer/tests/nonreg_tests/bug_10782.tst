// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 10782 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10782
//
// <-- Short Description -->
// iconvert did not check input arguments
//

B = 69;

b = int32([1 -120 127 312]);
REF = int8([1 -120 127 312]);
y = iconvert(b, 1);
assert_checkequal(y, REF);

y = iconvert(B, 0); // double
assert_checkequal(type(y), 1);
assert_checkequal(typeof(y), "constant");

y = iconvert(B, 1); // int8
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "int8");

y = iconvert(B, 2); // int16
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "int16");

y = iconvert(B, 4); // int32
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "int32");

y = iconvert(B, 11); // uint8
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "uint8");

y = iconvert(B, 12); // uint16
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "uint16");

y = iconvert(B, 14); // uint32
assert_checkequal(type(y), 8);
assert_checkequal(typeof(y), "uint32");

msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "iconvert", 2,"0, 1, 2, 4, 11, 12, 14");
assert_checkerror ("y = iconvert(B, 20);", msgerr);

msgerr = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "iconvert", 1);
assert_checkerror ("y = iconvert(%i, 0);", msgerr);

msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: An integer value expected.\n"), "iconvert", 2);
assert_checkerror ("y = iconvert(B, 0.1);", msgerr);

msgerr = msprintf(gettext("%s: Wrong size for argument #%d: Real scalar expected.\n"), "iconvert", 2);
assert_checkerror ("y = iconvert(B, [0 1]);", msgerr);
