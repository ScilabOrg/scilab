// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC --> 
//
// <-- Non-regression test for bug 5073 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5073
//
// <-- Short Description -->
// Optional argument added for decimal separator

assert_checkequal(strtod("1,3",","),1.3);
str_ref=",3";
d_ref=1;
[d, str]=strtod("1,3",".");
assert_checkequal(d, 1);
assert_checkequal(str, ",3");
